#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <stdexcept>
#include <entt/entt.hpp>
#include "mevent.hpp"
#include "train_base.hpp"

using namespace sf;
using namespace std;


namespace train_e {

    enum SpriteSlot {
		wheelFF = 0,
		wheelFR = 1, 
		wheelRF = 2,
		wheelRR = 3,
		bogeyFront = 4,
		bogeyRear = 5,
		Coupler = 6,
        DoorRL = 0+7,
        DoorRR = 1+7,
        Salon = 2+7, 
        Fdoor =2+8, 
        DoorLL = 3+8,
        DoorLR = 4+8,
        Body = 5+8,
    };

    enum class WireIndex {
        VUD = 15,
        DoorsLeft = 30,
        DoorsRight = 31
    };

    struct DoorSystem {
        bool LdoorsOpen = false;
        bool RdoorsOpen = false;
        bool doorsClosed = true; 
    };

    
    struct VudLamp {
        bool visible = false;
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
    struct Controller {
        int pos = 0;
    };
}
inline entt::entity makeWagonE(entt::registry& reg,
    RenderWindow* window,
    TextureManager& tm,
    bool isHead = false)
{
    entt::entity e = reg.create();

    using Tx = Texture&;

    Tx texBody = tm.get("textures\\wagons\\E\\body.png");
    Tx texSalon = tm.get("textures\\wagons\\E\\salon.png");
    Tx texDoorLL = tm.get("textures\\wagons\\E\\doorsLL.png");
    Tx texDoorLR = tm.get("textures\\wagons\\E\\doorsLR.png");
    Tx texDoorRL = tm.get("textures\\wagons\\E\\doorsRL.png");
    Tx texDoorRR = tm.get("textures\\wagons\\E\\doorsRR.png");
    Tx couplerTex = tm.get("textures\\wagons\\E\\scep.png");
    Tx bogeyFront = tm.get("textures\\wagons\\E\\bogeyF.png");
    Tx bogeyRear = tm.get("textures\\wagons\\E\\bogeyR.png");
    Tx wheelFF = tm.get("textures\\wagons\\E\\wheelFF.png");
    Tx wheelFR = tm.get("textures\\wagons\\E\\wheelFR.png");
    Tx wheelRF = tm.get("textures\\wagons\\E\\wheelRF.png");
    Tx wheelRR = tm.get("textures\\wagons\\E\\wheelRR.png");
	Tx Fdoor = tm.get("textures\\wagons\\E\\door_front.png");
    reg.emplace<train_base::RelPos>(e);
    reg.emplace<train_base::Movement>(e);
    reg.emplace<train_base::WireBus>(e);
    reg.emplace<train_base::PressureLines>(e);
    reg.emplace<train_base::Length>(e);
    reg.emplace<train_base::Scale>(e);
    reg.emplace<train_base::EventBuffer>(e);

    auto& spl = reg.emplace<train_base::SpriteList>(e);
    spl.add(Sprite(wheelFF));
    spl.add(Sprite(wheelFR));
    spl.add(Sprite(wheelRF));
    spl.add(Sprite(wheelRR));
    spl.add(Sprite(bogeyFront));
    spl.add(Sprite(bogeyRear));
    spl.add(Sprite(couplerTex));
    spl.add(Sprite(texDoorRL), animDrive(0, -50, 0.4f, animDrive::EaseType::Linear, true));
    spl.add(Sprite(texDoorRR), animDrive(0, 50, 0.4f, animDrive::EaseType::Linear, true));
    spl.add(Sprite(texSalon));
	spl.add(Sprite(Fdoor));
    spl.add(Sprite(texDoorLL), animDrive(0, -50, 0.4f, animDrive::EaseType::Linear, true));
    spl.add(Sprite(texDoorLR), animDrive(0, 50, 0.4f, animDrive::EaseType::Linear, true));
    spl.add(Sprite(texBody));
    //wheel diam 171-153 (pixels in texture)
    // ibis paint measurements (Y, invertedX) → converted to SFML (X, Y):
    //wheel FF center: ibis(171,679) → SFML(115, 171)
    //wheel FR center: ibis(171,597) → SFML(197, 171)
    //wheel RF center: ibis(171,198) → SFML(596, 171)
    //wheel RR center: ibis(171,116) → SFML(678, 171)

    using S = train_e::SpriteSlot;
    // Wheel textures are full-canvas layers (same 794px canvas as body).
    // ibis(Y, invertedX) → SFML(X=794-ibis2, Y=ibis1):
    //   wheelFF: canvas(115, 171)   wheelFR: canvas(197, 171)
    //   wheelRF: canvas(596, 171)   wheelRR: canvas(678, 171)
    //
    // SFML: origin is placed at 'position', so:
    //   top-left of sprite = position - origin * scale
    // For full-canvas layer (top-left must sit at wagPos):
    //   wagPos = (relPos + wagPos) - origin * scale  →  relPos = origin * scale (scale=2)
    spl.sprites[S::wheelFF].sprite.setOrigin(Vector2f(115, 171));
    spl.sprites[S::wheelFR].sprite.setOrigin(Vector2f(197, 171));
    spl.sprites[S::wheelRF].sprite.setOrigin(Vector2f(596, 171));
    spl.sprites[S::wheelRR].sprite.setOrigin(Vector2f(678, 171));
    spl.sprites[S::wheelFF].relPos = Vector2f(115 * 2, 171 * 2);  // = (230, 342)
    spl.sprites[S::wheelFR].relPos = Vector2f(197 * 2, 171 * 2);  // = (394, 342)
    spl.sprites[S::wheelRF].relPos = Vector2f(596 * 2, 171 * 2);  // = (1192, 342)
    spl.sprites[S::wheelRR].relPos = Vector2f(678 * 2, 171 * 2);  // = (1356, 342)

    reg.emplace<train_e::DoorSystem>(e);
    reg.emplace<train_e::VudLamp>(e);
    reg.emplace<train_e::Controller>(e);

    
    if (isHead) {
        reg.emplace<train_base::IsHead>(e);

        Texture& texPanel = tm.get("textures\\wagons\\E\\interface\\panel.png");
        Texture& texSw3 = tm.get("textures\\wagons\\E\\interface\\right_switch_off.png");
        Texture& texSw3on = tm.get("textures\\wagons\\E\\interface\\right_switch_on.png");
        Texture& texVUD = tm.get("textures\\wagons\\E\\interface\\VUD_light.png");
        Texture& texControllerBase = tm.get("textures\\wagons\\E\\interface\\controller_base.png");
        Texture& texControllerHandle = tm.get("textures\\wagons\\E\\interface\\controller_handle.png");
		Texture& texControllerTop = tm.get("textures\\wagons\\E\\interface\\controller_top.png");
		Texture& speedGauge = tm.get("textures\\wagons\\E\\interface\\speed_gauge.png");
		Texture& speedNeedle = tm.get("textures\\wagons\\E\\interface\\speed_arrow.png");

        auto& ui = reg.emplace<train_base::TrainUi>(e);
        if (!ui.font.openFromFile("fonts/consolas.ttf"))
            throw runtime_error("Failed to load font");


        Sprite pSpr(texPanel);
        pSpr.setScale(Vector2f(0.5f, 0.5f));
        pSpr.setPosition(Vector2f(760, 680));
        ui.addUISprite(pSpr);                           

        Sprite vudSpr(texVUD);
        vudSpr.setPosition(pSpr.getPosition());
        vudSpr.setScale(pSpr.getScale());
        ui.addUISprite(vudSpr);                         

        Vector2f pPos = pSpr.getPosition();

        
        entt::registry* regPtr = &reg;
        Vector2f leverSize(400,200 );         
        Vector2f leverPos(390, 190);          
        ui.addLever(Vector2f(-150, (window->getSize().y-(593))),leverSize,leverPos, Color::Transparent, ui.font, "", window,
            texControllerHandle, Vector2f(0, 0),
            texControllerBase, Vector2f(0, 0),
            Vector2f(396, 322), 115, 312-360-115, 7,
            [regPtr, e]() {

                auto& ui = regPtr->get<train_base::TrainUi>(e);
                if (ui.levers.empty()) return;
                int leverPos = ui.levers[0].pos;
                regPtr->get<train_e::Controller>(e).pos = leverPos - 3;
            }
		);
        ui.levers[0].startUpdatePos(3);
        ui.addGauge(Vector2f(300, 300), Vector2f(window->getSize().x -300, 0), window,
            speedGauge, speedNeedle, Vector2f(0, 0), Vector2f(241, 240),
			0.f, 100.f, 0, 90, 10
		);
        ui.addButton(
            Vector2f(50, 50),
            Vector2f(pPos.x + 104, pPos.y + 260),
            Color::Transparent, ui.font, "", window,
            [regPtr, e]() { regPtr->get<train_e::DoorSystem>(e).LdoorsOpen = true; }
        );
        ui.addButton(
            Vector2f(46, 45),
            Vector2f(pPos.x + 335.5f, pPos.y + 197.5f),
            Color::Transparent, ui.font, "", window,
            [regPtr, e]() { regPtr->get<train_e::DoorSystem>(e).RdoorsOpen = true; }
        );
        ui.addSwitch(
            Vector2f(49.5f, 46.5f),
            Vector2f(pPos.x + 317.5f, pPos.y + 294.f),
            ui.font, "VUD", window,
            [regPtr, e]() {
                auto& ds = regPtr->get<train_e::DoorSystem>(e);
                ds.doorsClosed = !ds.doorsClosed;
                if (ds.doorsClosed) { ds.LdoorsOpen = false; ds.RdoorsOpen = false; }
            },
            mg::emptyTex, texSw3, texSw3on
        );

        auto& kl = reg.emplace<train_e::KeyListener>(e);
        entt::registry* regPtr2 = &reg;

        kl.bind(Keyboard::Key::W, [regPtr2, e]() {
            int &pos = regPtr2->get<train_e::Controller>(e).pos;
			pos += 1;
			if (pos > 3) pos = 3;
            auto& uil = regPtr2->get<train_base::TrainUi>(e);
			uil.levers[0].startUpdatePos(pos + 3);
        });
        kl.bind(Keyboard::Key::S, [regPtr2, e]() {
            int& pos = regPtr2->get<train_e::Controller>(e).pos;
            pos -= 1;
            if (pos < -3) pos = -3;
            auto& uil = regPtr2->get<train_base::TrainUi>(e);
            uil.levers[0].startUpdatePos(pos + 3);
        });
        kl.bind(Keyboard::Key::V, [regPtr2, e]() {
            auto& ds = regPtr2->get<train_e::DoorSystem>(e);
            ds.doorsClosed = !ds.doorsClosed;
            if (ds.doorsClosed) { ds.LdoorsOpen = false; ds.RdoorsOpen = false; }
            auto& ui = regPtr2->get<train_base::TrainUi>(e);
            if (!ui.switches.empty()) ui.switches[0].tick();
        });
        kl.bind(Keyboard::Key::A, [regPtr2, e]() {
            auto& ds = regPtr2->get<train_e::DoorSystem>(e);
            if (!ds.doorsClosed) ds.LdoorsOpen = true;
        });
        kl.bind(Keyboard::Key::D, [regPtr2, e]() {
            auto& ds = regPtr2->get<train_e::DoorSystem>(e);
            if (!ds.doorsClosed) ds.RdoorsOpen = true;
        });
        
    }

    return e;
}

namespace train_e_systems {

   
    inline void updateUiSprites(train_base::TrainUi& ui) {
        if (ui.uiSprites.size() < 2 || ui.switches.empty()) return;
        ui.switches[0].check.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[0].check.setScale(ui.uiSprites[0].getScale());
        ui.switches[0].NegativeCheck.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[0].NegativeCheck.setScale(ui.uiSprites[0].getScale());
    }

   
    inline void simWagonE(entt::registry& reg,
        entt::entity e,
        const vector<MEvent>& input,
        double dt)
    {
        auto& mv   = reg.get<train_base::Movement>(e);
        auto& ds   = reg.get<train_e::DoorSystem>(e);
        auto& spl  = reg.get<train_base::SpriteList>(e);
        auto& rp   = reg.get<train_base::RelPos>(e);
        auto& lamp = reg.get<train_e::VudLamp>(e);
        auto& wb   = reg.get<train_base::WireBus>(e);

        using W = train_e::WireIndex;
        bool isHead = reg.all_of<train_base::IsHead>(e);

        //wheel diam 171-153 
        //wheel1 center 171 679
        //wheel2 center 171 597
        //wheel3 center 171 198
        //wheel3 center 171 116


        if (isHead) {
            auto& ui = reg.get<train_base::TrainUi>(e);
			
            updateUiSprites(ui);
            reg.get<train_e::KeyListener>(e).process(input);
			mv.accel = reg.get<train_e::Controller>(e).pos * .4f;
			mv.speed += mv.accel*dt;
			mv.distanceTaken = false;
            ui.gauges[0].setValue(abs(mv.speed));
            wb.wires[(int)W::VUD].self        = ds.doorsClosed ? 1 : 0;
            wb.wires[(int)W::DoorsLeft].self  = ds.LdoorsOpen  ? 1 : 0;
            wb.wires[(int)W::DoorsRight].self = ds.RdoorsOpen  ? 1 : 0;
        }
        else {
           
            bool vudFromWire   = wb.wires[(int)W::VUD].diff > 0;
            bool leftFromWire  = wb.wires[(int)W::DoorsLeft].diff > 0;
            bool rightFromWire = wb.wires[(int)W::DoorsRight].diff > 0;

            ds.doorsClosed = vudFromWire;
            if (ds.doorsClosed) {
                ds.LdoorsOpen = false;
                ds.RdoorsOpen = false;
            } else {
                ds.LdoorsOpen = leftFromWire;
                ds.RdoorsOpen = rightFromWire;
            }
        }
        using S = train_e::SpriteSlot;
        float deltaAngle = (mv.speed * METER_TO_PX * dt * 180.0f) / (M_PI * 171 - 153);
        spl.sprites[S::wheelFF].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelFF].sprite.getRotation());
        spl.sprites[S::wheelFR].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelFR].sprite.getRotation());
        spl.sprites[S::wheelRF].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelRF].sprite.getRotation());
        spl.sprites[S::wheelRR].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelRR].sprite.getRotation());
       
        using S = train_e::SpriteSlot;
        auto& s0 = spl.sprites[S::DoorRL];
        auto& s1 = spl.sprites[S::DoorRR];
        auto& s3 = spl.sprites[S::DoorLL];
        auto& s4 = spl.sprites[S::DoorLR];

        bool allBack   = s0.anim.backFinished && s1.anim.backFinished &&
                         s3.anim.backFinished && s4.anim.backFinished;
        bool anyMoving = (!s0.anim.finished || !s1.anim.finished ||
                          !s3.anim.finished || !s4.anim.finished) &&
                         (!s0.anim.backFinished || !s1.anim.backFinished ||
                          !s3.anim.backFinished || !s4.anim.backFinished);
        lamp.visible = !allBack || anyMoving;

       
        if (isHead) {
            auto& ui = reg.get<train_base::TrainUi>(e);
            if (ui.uiSprites.size() >= 2) {
                ui.uiSprites[1].setPosition(
                    lamp.visible ? ui.uiSprites[0].getPosition()
                                 : Vector2f(-1000, -1000));
            }
        }

       
        s0.updateAnim(dt, ds.RdoorsOpen);
        s1.updateAnim(dt, ds.RdoorsOpen);
        s3.updateAnim(dt, ds.LdoorsOpen);
        s4.updateAnim(dt, ds.LdoorsOpen);

        
        spl.updatePositions(rp.pos);
    }


    inline void simAllWagonsE(entt::registry& reg,
        const vector<MEvent>& input,
        double dt)
    {

        auto view = reg.view<train_e::DoorSystem>();
        for (auto e : view)
            simWagonE(reg, e, input, dt);
    }

} 