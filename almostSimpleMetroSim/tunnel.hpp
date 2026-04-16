// 438 772
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <deque>
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
    std::deque<tunnel> tunnels;
    RenderWindow* window;
    Texture tunnelTexture;
    float texXStart = 438.f;
    float texXEnd = 772.f;
    bool generatedFirst = false;
    mutex mutex;
public:
    tunnelSet(RenderWindow* window, Texture& tex, Vector2f offsets)
        : window(window), tunnelTexture(tex) {
        texXStart = offsets.x;
        texXEnd = offsets.y;
    }
    float getWidth() const { return texXEnd - texXStart; }

    void generateTunnel() {
        float y = window->getSize().y / 2.f;
        float w = getWidth();
        float screenWidth = window->getSize().x;
        float margin = 500.f;

        while (!tunnels.empty() && tunnels.back().spr.getPosition().x > screenWidth + margin) {
            tunnels.pop_back();
        }
        while (!tunnels.empty() && tunnels.front().spr.getPosition().x < -margin - w) {
            tunnels.pop_front();
        }
        while (tunnels.empty() || tunnels.front().spr.getPosition().x > -margin) {
            float newX = tunnels.empty() ? 0.f : tunnels.front().spr.getPosition().x - w;

            tunnel t(tunnelTexture, texXStart, texXEnd);
            t.spr.setPosition(Vector2f(newX, y));
            t.spr.setScale(Vector2f(2, 2));
            tunnels.push_front(t);

            while (tunnels.back().spr.getPosition().x < screenWidth + margin) {
                float newX = tunnels.back().spr.getPosition().x + w;

                tunnel t(tunnelTexture, texXStart, texXEnd);
                t.spr.setPosition(Vector2f(newX, y));
                t.spr.setScale(Vector2f(2, 2));
                tunnels.push_back(t);
            }
        }
    }
    void moveTunnels(Vector2f offset) {

        for (tunnel& T : tunnels) {
            T.spr.move(offset);
        }
    }
    void simulate(float offset) {
        lock_guard<std::mutex> lock(mutex);
        moveTunnels(Vector2f(offset, 0.f));
        generateTunnel();
    }
    void draw() {
        std::lock_guard<std::mutex> lock(mutex);
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
