#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

using namespace std;

class TextureManager {
public:
    sf::Texture& get(const string& path) {
        auto& tex = textures[path];
        if (tex.getSize().x == 0) tex.loadFromFile(path);
        return tex;
    }
    void forgetTexture(const string& name) { textures.erase(name); }
private:
    unordered_map<string, sf::Texture> textures;
};