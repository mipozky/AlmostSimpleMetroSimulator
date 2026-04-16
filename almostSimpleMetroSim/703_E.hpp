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
    //
    // 1  Ход (маневровый)
    /*2  Ход(последовательный)
        3  Ход(параллельный)
        4  Назад
        5  Вперед
        6  Торможение
        7  Свободный
        8  Замещение пневматическим торможением
        9  "Плюс" аккумуляторной батареи
        10  "Плюс" аккумуляторной батареи
        11  Аварийное освещение
        12  Сцепление
        13  Радиофикация
        14  Замещение пневматическим торможением
        15  Дверная сигнализация
        16  Закрывание дверей
        17  Возврат РП
        18  Сигнальные лампы
        19  Торможение
        20  Ослабление поля
        21  Ход(маневровый)
        22  Синхронизирующий провод
        23  Мотор компрессора
        24  Сигнализация неисправности
        25  Ручное торможение
        26  Радиофикация
        27  Освещение включено
        28  Освещение отключено
        29  Назад
        30  Вперед
        31  Открывание левых дверей
        32  Открывание правых дверей*/
    //
    enum class WireIndex {
		X1_T1 = 1,
		X2_T1a = 2,
		X3_T2 = 3,
		Backward = 4,
		Forward = 5,
		Brake = 6,
		//7 is free
		BrakeReplPneum = 8,
		BattaryPlus = 9,
		//10 is also BattaryPlus but irrelevant is simulation
		EmergencyLight = 11,
		Coupling = 12,
		Radio = 13,
		//14 is also BrakeReplPneum but irrelevant is simulation
		DoorFeedback = 15, // VUD
		DoorClose = 16,
		VRP = 17, //
		SignalLamps = 18,
		//19 is also Brake but irrelevant is simulation
		FieldWeakening = 20,
		//21 is also X1 but irrelevant is simulation
		SynchWire = 22,
		Compressor = 23,
		Malfunction = 24,
		HandBrake = 25,
		//26 is also Radio but irrelevant is simulation
		LightOn = 27,
		LightOff = 28,
		//29 is also Backward but irrelevant is simulation
		//30 is also Forward but irrelevant is simulation
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
    AssetManager& tm,
    bool isHead = false)
{
    entt::entity e = reg.create();

    using Tx = Texture&;

    Tx texBody = tm.get<Texture>("textures\\wagons\\E\\body.png");
    Tx texSalon = tm.get<Texture>("textures\\wagons\\E\\salon.png");
    Tx texDoorLL = tm.get<Texture>("textures\\wagons\\E\\doorsLL.png");
    Tx texDoorLR = tm.get<Texture>("textures\\wagons\\E\\doorsLR.png");
    Tx texDoorRL = tm.get<Texture>("textures\\wagons\\E\\doorsRL.png");
    Tx texDoorRR = tm.get<Texture>("textures\\wagons\\E\\doorsRR.png");
    Tx couplerTex = tm.get<Texture>("textures\\wagons\\E\\scep.png");
    Tx bogeyFront = tm.get<Texture>("textures\\wagons\\E\\bogeyF.png");
    Tx bogeyRear = tm.get<Texture>("textures\\wagons\\E\\bogeyR.png");
    Tx wheelFF = tm.get<Texture>("textures\\wagons\\E\\wheelFF.png");
    Tx wheelFR = tm.get<Texture>("textures\\wagons\\E\\wheelFR.png");
    Tx wheelRF = tm.get<Texture>("textures\\wagons\\E\\wheelRF.png");
    Tx wheelRR = tm.get<Texture>("textures\\wagons\\E\\wheelRR.png");
	Tx Fdoor = tm.get<Texture>("textures\\wagons\\E\\door_front.png");

    reg.emplace<train_base::RelPos>(e);
    reg.emplace<train_base::Movement>(e);
    reg.emplace<train_base::WireBus>(e);
    reg.emplace<train_base::PressureLines>(e);
    reg.emplace<train_base::Length>(e);
    reg.emplace<train_base::Scale>(e);
    reg.emplace<train_base::EventBuffer>(e);
	reg.emplace<train_base::WagonCount>(e);

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


    using S = train_e::SpriteSlot;
    spl.sprites[S::wheelFF].sprite.setOrigin(Vector2f(115, 171));
    spl.sprites[S::wheelFR].sprite.setOrigin(Vector2f(197, 171));
    spl.sprites[S::wheelRF].sprite.setOrigin(Vector2f(596, 171));
    spl.sprites[S::wheelRR].sprite.setOrigin(Vector2f(678, 171));
    spl.sprites[S::wheelFF].relPos = Vector2f(115 * 2, 171 * 2);  
    spl.sprites[S::wheelFR].relPos = Vector2f(197 * 2, 171 * 2); 
    spl.sprites[S::wheelRF].relPos = Vector2f(596 * 2, 171 * 2);
    spl.sprites[S::wheelRR].relPos = Vector2f(678 * 2, 171 * 2); 

    reg.emplace<train_e::DoorSystem>(e);
    reg.emplace<train_e::VudLamp>(e);
    reg.emplace<train_e::Controller>(e);

    
    if (isHead) {
        reg.emplace<train_base::IsHead>(e);

        Texture& texPanel = tm.get<Texture>("textures\\wagons\\E\\interface\\panel.png");
        Texture& texSw3 = tm.get<Texture>("textures\\wagons\\E\\interface\\right_switch_off.png");
        Texture& texSw3on = tm.get<Texture>("textures\\wagons\\E\\interface\\right_switch_on.png");
        Texture& texVUD = tm.get<Texture>("textures\\wagons\\E\\interface\\VUD_light.png");
        Texture& texControllerBase = tm.get<Texture>("textures\\wagons\\E\\interface\\controller_base.png");
        Texture& texControllerHandle = tm.get<Texture>("textures\\wagons\\E\\interface\\controller_handle.png");
        Texture& texControllerTop = tm.get<Texture>("textures\\wagons\\E\\interface\\controller_top.png");
        Texture& speedGauge = tm.get<Texture>("textures\\wagons\\E\\interface\\speed_gauge.png");
        Texture& speedNeedle = tm.get<Texture>("textures\\wagons\\E\\interface\\speed_arrow.png");

        auto& ui = reg.emplace<train_base::TrainUi>(e);
        Font& uifont = tm.get<Font>("fonts/consolas.ttf");

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
        Vector2f leverSize(400, 200);
        Vector2f leverPos(390, 190);
        ui.addLever(Vector2f(-150, (window->getSize().y - (593))), leverSize, leverPos, Color::Transparent, uifont, "", window,
            texControllerHandle, Vector2f(0, 0),
            texControllerBase, Vector2f(0, 0),
            Vector2f(396, 322), 115, 312 - 360 - 115, 7,
            [regPtr, e]() {

                auto& ui = regPtr->get<train_base::TrainUi>(e);
                if (ui.levers.empty()) return;
                int leverPos = ui.levers[0].pos;
                regPtr->get<train_e::Controller>(e).pos = leverPos - 3;
            }
        );
        ui.levers[0].startUpdatePos(3);
        ui.addGauge(Vector2f(300, 300), Vector2f(window->getSize().x - 300, 0), window,
            speedGauge, speedNeedle, Vector2f(0, 0), Vector2f(241, 240),
            0.f, 100.f, 0, 90, 10
        );
        ui.addButton(
            Vector2f(50, 50),
            Vector2f(pPos.x + 104, pPos.y + 260),
            Color::Transparent, uifont, "", window,
            [regPtr, e]() {
                auto& ds = regPtr->get<train_e::DoorSystem>(e);
                if (!ds.doorsClosed) { ds.LdoorsOpen = true; }
            }
        );
        ui.addButton(
            Vector2f(46, 45),
            Vector2f(pPos.x + 335.5f, pPos.y + 197.5f),
            Color::Transparent, uifont, "", window,
            [regPtr, e]() {
                auto& ds = regPtr->get<train_e::DoorSystem>(e);
                if (!ds.doorsClosed) { ds.RdoorsOpen = true; }
            }
        );
        ui.addButton(//osveschenie vklucheno
            Vector2f(50, 50),
            Vector2f(pPos.x + 33, pPos.y + 394),
            Color::Transparent, uifont, "", window,
            mg::nothing
        );
        ui.addButton(//osveschenie otkl-zvonok
            Vector2f(50, 50),
            Vector2f(pPos.x + 158, pPos.y + 394),
            Color::Transparent, uifont, "", window,
            mg::nothing
        );
        ui.addButton(//vozvrat rp
            Vector2f(50, 50),
            Vector2f(pPos.x + 283, pPos.y + 394),
            Color::Transparent, uifont, "", window,
            mg::nothing
        );
        ui.addButton(//signalizaciya neispravnosti
            Vector2f(50, 50),
            Vector2f(pPos.x + 416, pPos.y + 394),
            Color::Transparent, uifont, "", window,
            mg::nothing
        );
        ui.addButton(//signalizaciya dverey
            Vector2f(50, 50),
            Vector2f(pPos.x + 543, pPos.y + 394),
            Color::Transparent, uifont, "", window,
            mg::nothing
        );
        ui.addButton(// i dunno
            Vector2f(50, 50),
            Vector2f(pPos.x + 495, pPos.y + 524),
            Color::Transparent, uifont, "", window,
            mg::nothing
        );
        ui.addSwitch(
            Vector2f(49.5f, 46.5f),
            Vector2f(pPos.x + 317.5f, pPos.y + 294.f),
            uifont, "VUD", window,
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
            int& pos = regPtr2->get<train_e::Controller>(e).pos;
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
        kl.bind(Keyboard::Key::I, [regPtr2]() {
            auto view = regPtr2->view<train_base::SpriteList, train_e::DoorSystem>();
            for (auto ent : view) {
                auto& spl = view.get<train_base::SpriteList>(ent);
                auto& bodySpr = spl.sprites[train_e::SpriteSlot::Body].sprite;
                
                bool isVisible = (bodySpr.getColor().a == 255);
                Color newColor = isVisible ? Color(255, 255, 255, 0) : Color(255, 255, 255, 255);
                
                bodySpr.setColor(newColor);
                spl.sprites[train_e::SpriteSlot::DoorLL].sprite.setColor(newColor);
                spl.sprites[train_e::SpriteSlot::DoorLR].sprite.setColor(newColor);
                spl.sprites[train_e::SpriteSlot::DoorRL].sprite.setColor(newColor);
                spl.sprites[train_e::SpriteSlot::DoorRR].sprite.setColor(newColor);
                spl.sprites[train_e::SpriteSlot::Fdoor].sprite.setColor(newColor);
            }
            });

    }

    return e;
}

namespace train_e_systems {
    namespace wire_bus {

        inline void writeWire(train_base::WireBus& wb, train_e::WireIndex w, int val) {
            wb.local[(int)w].val = val;
        }
        inline int readWire(const train_base::WireBus& wb, train_e::WireIndex w) {
            return wb.train[(int)w].val;
        }
    }
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
		auto& wagCount = reg.get<train_base::WagonCount>(e);

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
            wire_bus::writeWire(wb, W::DoorClose, ds.doorsClosed ? 1 : 0);
            wire_bus::writeWire(wb, W::DoorsLeft, ds.LdoorsOpen ? 1 : 0);
            wire_bus::writeWire(wb, W::DoorsRight, ds.RdoorsOpen ? 1 : 0);
        }
        else {
            bool doorFeedbackFromWire = wire_bus::readWire(wb, W::DoorClose) > 0;
            bool leftFromWire = wire_bus::readWire(wb, W::DoorsLeft) > 0;
            bool rightFromWire = wire_bus::readWire(wb, W::DoorsRight) > 0;

            ds.doorsClosed = doorFeedbackFromWire;
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

        wire_bus::writeWire(wb, W::DoorFeedback, lamp.visible ? 1 : 0);
        if (isHead) {
            auto& ui = reg.get<train_base::TrainUi>(e);
            if (ui.uiSprites.size() >= 2) {
                ui.uiSprites[1].setPosition(wire_bus::readWire(wb, W::DoorFeedback) == 0 ? ui.uiSprites[0].getPosition() : Vector2f(-1000, -1000));            
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