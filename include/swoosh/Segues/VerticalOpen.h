#pragma once
#include <swoosh/Swoosh/Segue.h>
#include <swoosh/Swoosh/Game.h>
#include <swoosh/Swoosh/Ease.h>

using namespace swoosh;


/**
  @class VerticalOpen
  @brief Slices the screen in half vertical and moves the pieces left and right revealing the next scene behind them

  Behavior is the same across all quality modes
*/

class VerticalOpen : public Segue {
private:
  sf::Vector2u windowSize;
public:
 void onDraw(sf::RenderTexture& surface) override {
    double elapsed = getElapsed().asMilliseconds();
    double duration = getDuration().asMilliseconds();
    double alpha = ease::linear(elapsed, duration, 1.0);

    this->drawLastActivity(surface);

    surface.display(); // flip and ready the buffer

    sf::Texture temp(surface.getTexture()); // Make a copy of the source texture

    sf::Sprite left(temp); 
    const int halfW = (int)(windowSize.x / 2.0f);
    left.setTextureRect(sf::IntRect({0, 0}, {halfW, (int)windowSize.y}));
    left.setPosition(sf::Vector2f{(float)-alpha * (float)left.getTextureRect().size.x, 0.0f});

    sf::Sprite right(temp);
    right.setTextureRect(sf::IntRect({halfW, 0}, {(int)windowSize.x - halfW, (int)windowSize.y}));
    right.setPosition(sf::Vector2f{(float)(windowSize.x / 2.0f) + ((float)alpha * (float)right.getTextureRect().size.x), 0.0f});

    surface.clear();

    this->drawNextActivity(surface);

    surface.display(); // flip and ready the buffer
    sf::Texture temp2(surface.getTexture());
    sf::Sprite next(temp2);

    surface.draw(next);
    surface.draw(left);
    surface.draw(right);
  }

  VerticalOpen(sf::Time duration, Activity* last, Activity* next) : Segue(duration, last, next) {
    /* ... */ 
    windowSize = getController().getVirtualWindowSize();
  }

  ~VerticalOpen() {}
};
