#pragma once
#include <swoosh/Swoosh/Segue.h>
#include <swoosh/Swoosh/Game.h>
#include <swoosh/Swoosh/Ease.h>

using namespace swoosh;

/**
  @class HorizontalOpen
  @brief Splits the screen into upper and lower halfs and then moves the pieces up and down as if opening

  The effect is the same across all optimized modes
*/
class HorizontalOpen : public Segue {
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

    sf::Sprite top(temp); 
    const int halfH = (int)(windowSize.y / 2.0);
    top.setTextureRect(sf::IntRect({0, 0}, {(int)windowSize.x, halfH}));
    top.setPosition(sf::Vector2f{0.0f, (float)(-alpha * top.getTextureRect().size.y)});

    sf::Sprite bottom(temp);
    bottom.setTextureRect(sf::IntRect({0, halfH}, {(int)windowSize.x, (int)windowSize.y - halfH}));
    bottom.setPosition(sf::Vector2f{0.0f, (float)(windowSize.y / 2.0f) + ((float)alpha * (float)bottom.getTextureRect().size.y)});

    surface.clear();

    this->drawNextActivity(surface);

    surface.display(); // flip and ready the buffer
    sf::Texture temp2(surface.getTexture());
    sf::Sprite right(temp2);

    surface.draw(right);
    surface.draw(top);
    surface.draw(bottom);
  }

  HorizontalOpen(sf::Time duration, Activity* last, Activity* next) : Segue(duration, last, next) {
    /* ... */ 
    windowSize = getController().getVirtualWindowSize();
  }

  ~HorizontalOpen() { }
};
