




#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>

#include "mevent.hpp"
#include "MGraphics.hpp"

using namespace std;
using namespace sf;

struct animDrive {
    enum class EaseType { Linear, EaseIn, EaseOut, EaseInOut, Custom };

    animDrive() = default;
    animDrive(float start, float end, float dur,
        EaseType ease = EaseType::Linear,
        bool revEase = false, bool looped = false)
        : RelstartPos(start), RelendPos(end), RelcurPos(start),
        duration(dur), loop(looped), ease(ease), revEase(revEase) {
    }

    EaseType ease = EaseType::Linear;
    float RelstartPos = 0, RelendPos = 0, RelcurPos = 0;
    float duration = 0, elapsed = 0;
    bool  finished = false, backFinished = true, loop = false, revEase = false;
    float(*customEase)(float) = nullptr;

    float applyEase(float t) {
        EaseType e = ease;
        if (e == EaseType::EaseIn && revEase) e = EaseType::EaseOut;
        else if (e == EaseType::EaseOut && revEase) e = EaseType::EaseIn;
        switch (e) {
        case EaseType::Linear:    return t;
        case EaseType::EaseIn:    return t * t;
        case EaseType::EaseOut:   return t * (2 - t);
        case EaseType::EaseInOut: return t < 0.5f ? 2 * t * t : 1 - pow(-2 * t + 2, 2) / 2;
        case EaseType::Custom:    return customEase(t);
        default:                  return t;
        }
    }
    void stepForward(float dt) {
        if (finished) return;
        backFinished = false;
        elapsed += dt;
        if (duration <= 0 || elapsed >= duration) { RelcurPos = RelendPos; finished = true; return; }
        RelcurPos = RelstartPos + (RelendPos - RelstartPos) * applyEase(elapsed / duration);
    }
    void stepBackward(float dt) {
        if (backFinished) return;
        finished = false;
        elapsed -= dt;
        if (duration <= 0 || elapsed <= 0.f) { elapsed = 0; RelcurPos = RelstartPos; backFinished = true; return; }
        bool wasRev = revEase; revEase = !revEase;
        RelcurPos = RelstartPos + (RelendPos - RelstartPos) * applyEase(elapsed / duration);
        revEase = wasRev;
    }
};

class Ent_Train : public mg::DrawBase
{
protected:
    class UI {
    public:
        vector<mg::Button>  buttons;
        vector<mg::TickBox> switches;
        vector<mg::Lever>   levers;
        vector<mg::Gauge>   gauges;
        vector<Sprite>      uiSprites;

        void addButton(Vector2f size, Vector2f pos, Color color,
            const Font& font, string text,
            RenderWindow* window, function<void()> cb)
        {
            buttons.emplace_back(size, pos, color, font, text, window, cb);
        }
        void addLever(Vector2f pos, Vector2f handleColsize, Vector2f handleColpos, Color color, const Font& font, string text,
            RenderWindow* window, const Texture& handleSpr, Vector2f handleSpriteOffset,
            const Texture& baseSpr, Vector2f baseSpriteOffset, Vector2f hingeloc, float startAngle_,
            float moveAngle, int positions, function<void()> cb)
        {
            levers.emplace_back(Vector2f(0, 0), pos, handleColsize, handleColpos, color,
                text, window,
                handleSpr, handleSpriteOffset,
                baseSpr, baseSpriteOffset,
                hingeloc, startAngle_, moveAngle, positions, false, cb);
        }
        void addGauge(Vector2f size, Vector2f pos, RenderWindow* window,
            const Texture& base, const Texture& needleTex, Vector2f spriteOffset, Vector2f hinge,
            float minVal, float maxVal,
            float minAngle, float maxAngle,
            float physLimitDegrees,
            bool circular = false)
        {
            gauges.emplace_back(size, pos, window,
                base, needleTex, spriteOffset, hinge,
                minVal, maxVal, minAngle, maxAngle,
                physLimitDegrees, circular);
        }
        void addSwitch(Vector2f size, Vector2f pos, const Font& font,
            string text, RenderWindow* window, function<void()> cb,
            Texture& boxTex, Texture& checkTex, Texture& negTex)
        {
            switches.emplace_back(size, pos, font, text, window, cb,
                boxTex, checkTex, negTex);
        }
        void adduiSprite(const Sprite& spr) { uiSprites.push_back(spr); }

        void checkEvents(const Event& ev) {
            for (auto& b : buttons)  b.checkPress(ev);
            for (auto& s : switches) s.checkPress(ev);
            for (auto& l : levers)   l.CheckMovement(ev);
        }
        void draw(RenderWindow* window) {
            // quickLog("started to draw");
            for (auto& l : levers)    l.draw();
            for (auto& spr : uiSprites) window->draw(spr);
            // quickLog("drawn sprites");
            for (auto& b : buttons)   b.draw();
            // quickLog("drawn buttons");
            for (auto& s : switches)  s.draw();
            // quickLog("drawn switches");
            // quickLog("drawn levers");
            for (auto& g : gauges)    g.draw();
            // quickLog("drawn gauges");
        }
    };
    struct KeyListener {
        unordered_map<sf::Keyboard::Key, std::function<void()>> bindings;
        unordered_map<sf::Keyboard::Key, bool> pressedKeys;
        void bind(sf::Keyboard::Key key, std::function<void()> fn) {
            bindings[key] = std::move(fn);
            pressedKeys[key] = false;
        }

        void process(const std::vector<MEvent>& input) const {
            for (const auto& m : input) {
                if (m.sender != "window" || m.type != "KeyPressed") continue;
                auto it = bindings.find(m.key);
                if (it != bindings.end()) it->second();
            }
        }
    };
    KeyListener keyListener;
    struct fSprite {
        Sprite    sprite;
        Vector2f  relPos;
        animDrive anim;

        explicit fSprite(const Sprite& spr) : sprite(spr), relPos(spr.getPosition()) {}

        void updateAnim(float dt, bool forward = true) {
            if (forward) anim.stepForward(dt);
            else         anim.stepBackward(dt);
            relPos.x = anim.RelcurPos;
        }
        void Hide() {
            sprite.setColor(Color(255, 255, 255, 0));
        }
        void Show() {
            sprite.setColor(Color(255, 255, 255, 255));
        }
        void ToggleVisible() {
            if (sprite.getColor() == Color(255, 255, 255, 0)) Show();
            else Hide();
        }
        void SetVisible(bool visible) { if (visible) Show(); else Hide(); }
    };
    int mmlength = 20000;
    struct wireBus {

        array<float, 32> local{};
        array<float, 32> train{};

        vector <pair<int, int>> swaps{};
        bool swapON = false;
        void addSwap(int v1, int v2) {
            swaps.push_back(pair<int, int>{v1, v2});
        }
        void writeWire(int idx, int val) {
            if (swapON) {
                for (auto& pair : swaps) {
                    if (idx == pair.first) {
                        idx = pair.second;
                        break;
                    }
                    else if (idx == pair.second) {
                        idx = pair.first;
                        break;
                    }
                }
            }
            train[idx] += (val - local[idx]);
            local[idx] = val;
        }
        inline int readWire(int idx) {
            if (swapON) {
                for (auto& pair : swaps) {
                    if (idx == pair.first) { 
                        idx = pair.second;
                        break;
                    }
                    else if (idx == pair.second) {
                        idx = pair.first;
                        break;
                    }
                }
            }
            return train[idx];
        }
    };

    Vector2f scale;
    Vector2f pos{};
    int entityId;
    RenderWindow* window;
    wireBus wireBus;

    void drawBase() override {
        for (auto& s : spriteList.sprites) {
            window->draw(s.sprite);
        }
    }

    virtual vector<MEvent> simulate(vector<MEvent>* input, float dt) {
		//speed += accel * dt;
        return {};
    }
    float movedDistance = 0.0f;
    bool distanceTaken = false;

    struct button {
        bool value() {
            taken = true;
            return val;
        };
        bool taken = false;
        bool val = false;
        void process() {
            if (taken) val = false;
        }
    };
    
public:
    virtual ~Ent_Train() {
        std::ofstream file("force" + std::to_string(entityId) + ".txt", std::ios::app);

            if (file.is_open()) {
                string sum = "";
				for (const auto& force : forceHistory) {
					sum += std::to_string(force) + "\n";
				}
				file << sum;
				file.close();
            }
    };
    float main750v = 0;
    UI ui;
    struct SpriteList {
        vector<fSprite> sprites;

        void add(const Sprite& spr) { sprites.emplace_back(spr); }
        void add(const Sprite& spr, animDrive anim) {
            sprites.emplace_back(spr);
            sprites.back().anim = anim;
        }

        void updatePositions(Vector2f wagPos) {
            for (auto& s : sprites)
                s.sprite.setPosition(s.relPos + wagPos);
        }
        void applyScale(Vector2f sc) {
            for (auto& s : sprites)
                s.sprite.setScale(sc);
        }
    };
    SpriteList spriteList;
    float speed = 0.f;
    float accel = 0.f;
    int wagonCount = 1;

    float takeMovedDistance() { distanceTaken = !distanceTaken; return movedDistance; }
    bool isHead = false;
    vector<MEvent> events;

    Ent_Train(int id, sf::RenderWindow* window) {
        entityId = id;
        this->window = window;
    }

    Vector2f getPos() { return pos; }
    int      getId() { return entityId; }
    int      getSpriteCount() { return (int)spriteList.sprites.size(); }

    Sprite& getSprite(int id) { return spriteList.sprites[id].sprite; }

    array<float, 32> sendWires() { return wireBus.local; }
    void            setWires(array<float, 32> wires) { wireBus.train = wires; }



    void setScale(float coef = 1) {
        scale.x = scale.y = mmlength * coef / 10000.0f;
        for (auto& s : spriteList.sprites) s.sprite.setScale(scale);
    }

    void updatePos() {
        for (auto& s : spriteList.sprites) s.sprite.setPosition(s.relPos + pos);
    }

    void setPos(Vector2f newPos) {
        pos = newPos;
        updatePos();
    }

    vector<MEvent> getSentMev() { return events; }

    void sim(vector<MEvent>* input, float dt) {

        if (distanceTaken) movedDistance = 0.0f;
        vector<MEvent> res = simulate(input, dt);
        events.assign(res.begin(), res.end());
    }

    void checkPreses(const Event& ev) {
        if (!isHead) return;
        ui.checkEvents(ev);
    }
    void drawui() {
        if (!isHead) return;
        ui.draw(window);
    }
	float w1t1pos = 0.0f, w2t1pos = 0.0f, w1t2pos = 0.0f, w2t2pos = 0.0f;
    double TrainLine = 0;
    double BrakeLine = 0;
    bool TrainLineOpen = 0;
	bool brakeValveFront = 0, brakeValveRear = 0;
	bool trainValveFront = 0, trainValveRear = 0;
	bool reversed = false;
	int wagonId = 0;
	double netForce = 0.0;
	double mass = 0.0;
    int capacity = 0;
    vector <double> forceHistory;
protected:
    struct relay {
        bool   normallyClosed = false;
        double closeTime = 0.050;
        double openTime = 0.050;
        bool   pneumatic = false;
        double Time = 0.0;
        bool   hasChangeTime = false;
        double ChangeTime = 0.0;
        double Value = 0.0;
        bool value = 0;
        double TargetValue = 0.0; 
        double Blocked = 0.0;

        relay(bool normallyClosed = false)
            : normallyClosed(normallyClosed)
        {
            Value = TargetValue = normallyClosed ? 1.0 : 0.0;
        }
        void sim(double dt) {
            Time += dt;
            if (hasChangeTime && Time > ChangeTime) {
                Value = TargetValue;
                hasChangeTime = false;
            }
            if (Value > 0.5) value = 1;
            else value = 0;
        }
        void close(double linePressure = -1.0) {
            if (Blocked > 0.0) return;
            if (Value == 1.0 && TargetValue == 1.0) return;
            if (pneumatic && linePressure < 3.0) return;

            if (!hasChangeTime || TargetValue != 1.0) {
                ChangeTime = Time + closeTime;
                hasChangeTime = true;
            }
            if (Value == 1.0) hasChangeTime = false;

            TargetValue = 1.0;
        }
        void open() {
            if (Blocked > 0.0) return;
            if (Value == 0.0 && TargetValue == 0.0) return;

            if (!hasChangeTime || TargetValue != 0.0) {
                ChangeTime = Time + openTime;
                hasChangeTime = true;
            }
            if (Value == 0.0) hasChangeTime = false;

            TargetValue = 0.0;
        }
        void set(bool v, double linePressure = -1.0) {
            if (v) close(linePressure);
            else   open();
        }
    };
    struct LevelRelay : relay {
        double triggerLevel = 0.0;

        explicit LevelRelay(double level = 0.0, bool normallyClosed = false)
            : relay(normallyClosed), triggerLevel(level) {
        }
        void set(double magnitude) {
            relay::set(fabs(magnitude) > triggerLevel);
        }
    };
};