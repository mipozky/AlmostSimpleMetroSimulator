#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>
#include "mevent.hpp"
#include "MGraphics.hpp"

using namespace sf;
using namespace std;

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

struct wire { int self = 0, diff = 0; };


struct fSprite {
    Sprite    sprite;
    Vector2f  relPos;
    animDrive anim;
    bool      drawn = true;

    explicit fSprite(const Sprite& spr) : sprite(spr), relPos(spr.getPosition()) {}

    void updateAnim(float dt, bool forward = true) {
        if (forward) anim.stepForward(dt);
        else         anim.stepBackward(dt);
        relPos.x = anim.RelcurPos;
    }
};


namespace train_base {

    
    struct RelPos {
        Vector2f pos{};
    };

   
    struct Movement {
        bool  distanceTaken = false;
		float speed = 0.f;
		float accel = 0.f;
    };

    
    struct WireBus {
        array<wire, 32> wires{};
    };

    
    struct PressureLines {
        float trainLine = 0.f;
        float breakLine = 0.f;
    };

    
    struct Length {
        int mmlength = 20000;
    };

   
    struct Scale {
        Vector2f scale{ 1.f, 1.f };
    };

    
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

    
    struct IsHead {};

  
    struct EventBuffer {
        vector<MEvent> events;
    };
    struct TrainUi {
        vector<mg::Button>  buttons;
        vector<mg::TickBox> switches;
		vector<mg::Lever>   levers;
		vector<mg::Gauge>   gauges;
        vector<Sprite>      uiSprites;
        Font font;

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
            levers.emplace_back(Vector2f(0, 0), pos,  handleColsize, handleColpos,color,
                font, text, window,
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
        void addUISprite(const Sprite& spr) { uiSprites.push_back(spr); }

        void checkEvents(const Event& ev) {
            for (auto& b : buttons)  b.checkPress(ev);
            for (auto& s : switches) s.checkPress(ev);
			for (auto& l : levers)   l.CheckMovement(ev);
        }
        void draw(RenderWindow* window) {
            for (auto& spr : uiSprites) window->draw(spr);
            for (auto& b : buttons)   b.draw();
            for (auto& s : switches)  s.draw();
			for (auto& l : levers)    l.draw();
			for (auto& g : gauges)    g.draw();
        }
    };

}
namespace train_base_systems {

    
    inline void applyScale(entt::registry& reg, float coef = 1.f) {
        auto view = reg.view<train_base::Length, train_base::Scale, train_base::SpriteList>();
        for (auto e : view) {
            auto& len = view.get<train_base::Length>(e);
            auto& sc = view.get<train_base::Scale>(e);
            auto& spl = view.get<train_base::SpriteList>(e);
            float s = len.mmlength * coef / 10000.f;
            sc.scale = { s, s };
            spl.applyScale(sc.scale);
        }
    }

   
    inline void updatePositions(entt::registry& reg) {
        auto view = reg.view<train_base::RelPos, train_base::SpriteList>();
        for (auto e : view) {
            auto& rp = view.get<train_base::RelPos>(e);
            auto& spl = view.get<train_base::SpriteList>(e);
            spl.updatePositions(rp.pos);
        }
    }

    
    inline void drawAll(entt::registry& reg, RenderWindow& window) {
        auto view = reg.view<train_base::SpriteList>();
        for (auto e : view) {
            for (auto& s : view.get<train_base::SpriteList>(e).sprites)
                window.draw(s.sprite);
        }
    }

    inline void drawUI(entt::registry& reg, RenderWindow& window) {
        auto view = reg.view<train_base::IsHead, train_base::TrainUi>();
        for (auto e : view)
            view.get<train_base::TrainUi>(e).draw(&window);
    }

    
    inline void checkUIEvents(entt::registry& reg, const Event& ev) {
        auto view = reg.view<train_base::IsHead, train_base::TrainUi>();
        for (auto e : view)
            view.get<train_base::TrainUi>(e).checkEvents(ev);
    }


    inline void updateWires(entt::registry& reg, const vector<entt::entity>& order) {
        array<wire, 32> total{};
        for (auto e : order) {
            auto& wb = reg.get<train_base::WireBus>(e);
            for (int i = 0; i < 32; i++) total[i].diff += wb.wires[i].self;
        }
        for (auto e : order) {
            auto& wb = reg.get<train_base::WireBus>(e);
            array<wire, 32> set{};
            for (int i = 0; i < 32; i++) {
                set[i].diff = total[i].diff - wb.wires[i].self;
                set[i].self = wb.wires[i].self;
            }
            wb.wires = set;
        }
    }
    inline float readSpeed(entt::registry& reg, entt::entity e) {
        reg.get<train_base::Movement>(e).distanceTaken = true;
        return reg.get<train_base::Movement>(e).speed;
	}

} 