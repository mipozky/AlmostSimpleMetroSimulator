#pragma once
#include <string>
#include <SFML/Window/Keyboard.hpp>

struct MEvent {
    std::string type;
    std::string sender;
    std::string receiver;

    void* data = nullptr;

    sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
    bool alt     = false;
    bool control = false;
    bool shift   = false;
    bool system  = false;
};
