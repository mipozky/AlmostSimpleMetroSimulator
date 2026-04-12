// 438 772
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

struct tunnel {
  Sprite spr;
  float texXStart;
  float texXEnd;

  tunnel(Texture &tex, float texXStart, float texXEnd)
      : spr(tex), texXStart(texXStart), texXEnd(texXEnd) {
    spr.setOrigin(Vector2f(texXStart, tex.getSize().y / 2));
  }

  float getVisualWidth() const { return texXEnd - texXStart; }
};

class tunnelSet {
  std::vector<tunnel> tunnels;
  RenderWindow *window;
  Texture tunnelTexture;
  float texXStart = 438.f;
  float texXEnd = 772.f;
  bool generatedFirst = false;
public:
  tunnelSet(RenderWindow *window, Texture& tex, Vector2f offsets)
      : window(window), tunnelTexture(tex) {
    texXStart = offsets.x;
    texXEnd = offsets.y;
  }
  float getWidth() const { return texXEnd - texXStart; }

  void generateTunnel() {
    float y = window->getSize().y / 2.f;
    float w = getWidth();
    tunnels.erase(std::remove_if(tunnels.begin(), tunnels.end(),
                                 [&](const tunnel &T) {
                                   return T.spr.getPosition().x >
                                          (float)window->getSize().x+500;
                                 }),
                  tunnels.end());
    if (tunnels.empty() || tunnels.front().spr.getPosition().x > -500.f) {
      float newX =
          tunnels.empty() ? 0.f : tunnels.front().spr.getPosition().x - w;

      tunnel t(tunnelTexture, texXStart, texXEnd);
      t.spr.setPosition(Vector2f(newX, y));
      t.spr.setScale(Vector2f(2, 2));
      tunnels.insert(tunnels.begin(), t);
    }
  }

  void moveTunnels(Vector2f offset) {
    for (tunnel &T : tunnels) {
      T.spr.move(offset);
    }
  }
  void draw() {
      for (const tunnel& T : tunnels) {
          try {
              window->draw(T.spr);
          }
          catch 
          (const exception& e) {
			  throw runtime_error(string("Error drawing tunnel: ") + e.what());
		  }
      }
    }
  
};

//todo add mutex`