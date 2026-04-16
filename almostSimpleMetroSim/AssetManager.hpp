#pragma once
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

#include <SFML/Audio.hpp>
#include <variant>
#include <string>
#include <unordered_map>
#include <memory>
#include <type_traits>

class AssetManager {
    using loadFileType = std::variant<sf::Texture, sf::Image, sf::Shader, sf::SoundBuffer>;
    using openFileType = std::variant<sf::Font, std::unique_ptr<sf::Music>>;

    class loadFileAssetManager {
    public:
        template <typename T>
        T& get(const std::string& path) {
            auto it = assets.find(path);
            if (it == assets.end()) {
                T asset;
                if (!asset.loadFromFile(path))
                    throw std::runtime_error("Load failed: " + path);

                it = assets.emplace(path, std::move(asset)).first;
            }
            return std::get<T>(it->second);
        }
        void forget(const std::string& path) { assets.erase(path); }
    private:
        std::unordered_map<std::string, loadFileType> assets;
    };

    class openFileAssetManager {
    public:
        template <typename T>
        T& get(const std::string& path) {
            auto it = assets.find(path);
            if (it == assets.end()) {
                if constexpr (is_same_v<T, sf::Music>) {
                    auto music = std::make_unique<sf::Music>();
                    if (!music->openFromFile(path))
                        throw std::runtime_error("Open failed: " + path);
                    it = assets.emplace(path, std::move(music)).first;
                }
                else {
                    T asset;
                    if (!asset.openFromFile(path))
                        throw std::runtime_error("Open failed: " + path);
                    it = assets.emplace(path, std::move(asset)).first;
                }
            }

            if constexpr (std::is_same_v<T, sf::Music>) {
                return *std::get<std::unique_ptr<sf::Music>>(it->second);
            }
            else {
                return std::get<T>(it->second);
            }
        }
        void forget(const std::string& path) { assets.erase(path); }
    private:
        std::unordered_map<std::string, openFileType> assets;
    };

    loadFileAssetManager loadFileManager;
    openFileAssetManager openFileManager;

public:
    template <typename T>
    T& get(const std::string& path) {
        if constexpr (std::is_same_v<T, sf::Font> || std::is_same_v<T, sf::Music>) {
            return openFileManager.get<T>(path);
        }
        else {
            return loadFileManager.get<T>(path);
        }
    }

    void forget(const std::string& path) {
        loadFileManager.forget(path);
        openFileManager.forget(path);
    }
};