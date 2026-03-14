#pragma once
#include <swoosh/Swoosh/Segue.h>
#include <swoosh/Swoosh/Game.h>
#include <swoosh/Swoosh/Ease.h>

using namespace swoosh;

/**
  @class HorizontalSlice
  @brief Splits the screen into upper and lower halfs and then sends the pieces in opposite directions, revealing the next scene

  The effect is the same across all optimized modes
*/
class HorizontalSlice : public Segue {
private:
  sf::Vector2u windowSize;
  int direction;
public:
 void onDraw(sf::RenderTexture& surface) override {
    double elapsed = getElapsed().asMilliseconds();
    double duration = getDuration().asMilliseconds();
    double alpha = 1.0 - ease::bezierPopOut(elapsed, duration);

    this->drawLastActivity(surface);

    surface.display(); // flip and ready the buffer

    sf::Texture temp(surface.getTexture()); // Make a copy of the source texture

    sf::Sprite top(temp); 
    top.setTextureRect(sf::IntRect({0, 0}, {(int)windowSize.x, (int)(windowSize.y / 2)}));
    top.setPosition(sf::Vector2f{(float)(direction * alpha * (double)top.getTexture().getSize().x), 0.0f});

    sf::Sprite bottom(temp);
    bottom.setTextureRect(sf::IntRect({0, (int)(windowSize.y / 2)}, {(int)windowSize.x, (int)windowSize.y}));
    bottom.setPosition(sf::Vector2f{(float)(direction * -alpha * (double)bottom.getTexture().getSize().x), (float)(windowSize.y/2.0f)});

    surface.clear();

    this->drawNextActivity(surface);

    surface.display(); // flip and ready the buffer

    sf::Texture temp2(surface.getTexture());
    sf::Sprite right(temp2);

    surface.draw(right);
    surface.draw(top);
    surface.draw(bottom);
  }

  HorizontalSlice(sf::Time duration, Activity* last, Activity* next) : Segue(duration, last, next) {
    /* ... */ 
    windowSize = getController().getVirtualWindowSize();
    direction = rand() % 2 == 0 ? -1 : 1;
  }

  ~HorizontalSlice() { }
};
