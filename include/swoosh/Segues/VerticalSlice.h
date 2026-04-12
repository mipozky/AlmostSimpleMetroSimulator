#pragma once
#include <swoosh/Swoosh/Segue.h>
#include <swoosh/Swoosh/Game.h>
#include <swoosh/Swoosh/Ease.h>

using namespace swoosh;

/**
  @class VerticalSlice
  @brief Slices the screen in half and moves the pieces in opposite vertical directions revealing the next scene behind them

  Behavior is the same across all quality modes
*/
class VerticalSlice : public Segue {
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

    const int halfW = (int)(windowSize.x / 2.0);
    sf::Sprite left(temp); 
    left.setTextureRect(sf::IntRect({0, 0}, {halfW, (int)windowSize.y}));
    left.setPosition(sf::Vector2f{0.0f, (float)(direction * alpha * (double)left.getTexture().getSize().y)});

    sf::Sprite right(temp);
    right.setTextureRect(sf::IntRect({halfW, 0}, {(int)windowSize.x - halfW, (int)windowSize.y}));
    right.setPosition(sf::Vector2f{(float)(windowSize.x/2.0f), (float)(direction * -alpha * (double)right.getTexture().getSize().y)});

    surface.clear();

    this->drawNextActivity(surface);

    surface.display(); // flip and ready the buffer

    sf::Texture temp2(surface.getTexture());
    sf::Sprite next(temp2);

    surface.draw(next);
    surface.draw(left);
    surface.draw(right);
  }

  VerticalSlice(sf::Time duration, Activity* last, Activity* next) : Segue(duration, last, next) {
    /* ... */ 
    windowSize = getController().getVirtualWindowSize();
    direction = rand() % 2 == 0 ? -1 : 1;
  }

  ~VerticalSlice() { }
};
