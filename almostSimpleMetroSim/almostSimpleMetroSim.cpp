#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>
#include <chrono>
#include <optional>
#include <vector>
#include <string>
#include <TGui/TGui.hpp>
#include <TGui/Backend/SFML-Graphics.hpp>
#include "console.hpp"
#define METER_TO_PX 71.1f
#include "includes.h"
#include "AssetManager.hpp"
#include "MGraphics.hpp"
#include "mevent.hpp"
#include "tunnel.hpp"
#include "train_base.hpp"
#include "703_E.hpp"
bool showfps = false;
bool simQuality = false;
#include <SFML/OpenGL.hpp>
#include <consoleapi3.h>
#include <windows.h>

using namespace std;
using namespace sf;

//#define KOSTIL

float simSpeed = 1.f;

void failureDraw(Console& console, tgui::Gui& gui, RenderWindow* window) {
    window->setActive(true);
    console.log("Fail draw enabled");
    HWND hwnd = window->getNativeHandle();
    while (window->isOpen()) {
        console.on();
        if (GetAsyncKeyState(VK_LWIN) & 0x8000 || GetAsyncKeyState(VK_RWIN) & 0x8000)
            ShowWindow(hwnd, SW_MINIMIZE);
        while (const auto event = window->pollEvent()) {
            gui.handleEvent(*event);
            if (event->is<sf::Event::Closed>()) window->close();
        }
        window->clear({ 20, 0, 0 });
        gui.draw();
        window->display();
    }
}
class MEventBus {
public:
    void emit(const MEvent& event) {
        lock_guard<mutex> lock(mutex_);
        eventLog.push_back(event);
    }
    void drainTo(vector<MEvent>& out) {
        lock_guard<mutex> lock(mutex_);
        if (eventLog.empty()) return;
        out.insert(out.end(), eventLog.begin(), eventLog.end());
        eventLog.clear();
    }
private:
    mutex mutex_;
    vector<MEvent> eventLog;
};

Ent_Train& findActiveHead(vector<unique_ptr<Ent_Train>>& cars) {
    for (auto& t : cars) {
        if (t->isHead == true) return *t;
    }
    return *cars[0];
}

struct Consist {
    vector<unique_ptr<Ent_Train>> cars;

    void updateWires() {
        array<float, 32> total{};
        for (auto& t : cars) {
            auto wires = t->sendWires();
            for (int i = 0; i < 32; i++)
                total[i] += wires[i];
        }
        for (auto& t : cars) {
            for (int i = 0; i < 32; i++)
                t->setWires(total);
        }
    }
    void writeWagonCount() {
        int count = cars.size();
        for (size_t i = 0; i < cars.size(); ++i) {
            cars[i]->wagonCount = count;
            cars[i]->wagonId = i;
        }
    }
 
    double equalizeCoupledPressure(float dT, double& selfP, double* otherP,
        bool valveOpen, float rate, float closeRate)
    {
        if (!valveOpen) return 0.0;

        double P2 = otherP ? *otherP : 0.0;
        if (!otherP) rate = (closeRate > 0.f) ? closeRate : rate;

        double dPdT = rate * (P2 - selfP);
        double dP = dPdT * dT;
        double P0 = (P2 + selfP) / 2.0;

        if (dP > 0) {
            selfP = min(P0, selfP + dP);
            if (otherP) *otherP = max(P0, *otherP - dP);
        }
        else {
            selfP = max(P0, selfP + dP);
            if (otherP) *otherP = min(P0, *otherP - dP);
        }
        return dP;
    }

    void getCoupledNeighbors(size_t i, Ent_Train** outFront, Ent_Train** outRear) {
        Ent_Train* self = cars[i].get();
        Ent_Train* towardsHead = (i > 0) ? cars[i - 1].get() : nullptr;
        Ent_Train* towardsTail = (i + 1 < cars.size()) ? cars[i + 1].get() : nullptr;

        if (self->reversed) {
            *outFront = towardsTail;
            *outRear = towardsHead;
        }
        else {
            *outFront = towardsHead;
            *outRear = towardsTail;
        }
    }

    void equalizeTrainLinePressure(float dT) {
        const float rate = 100.f;
        const float closeRate = 0.08f;

        for (size_t i = 0; i < cars.size(); i++) {
            Ent_Train* self = cars[i].get();
            Ent_Train* front;
            Ent_Train* rear;
            getCoupledNeighbors(i, &front, &rear);

            bool frontOpen = self->trainValveFront && front != nullptr;
            bool rearOpen = self->trainValveRear && rear != nullptr;

            double frontP = frontOpen ? front->TrainLine : 0.0;
            double rearP = rearOpen ? rear->TrainLine : 0.0;

            equalizeCoupledPressure(dT, self->TrainLine,
                frontOpen ? &frontP : nullptr, self->trainValveFront, rate, closeRate);
            if (frontOpen) front->TrainLine = frontP;

            equalizeCoupledPressure(dT, self->TrainLine,
                rearOpen ? &rearP : nullptr, self->trainValveRear, rate, closeRate);
            if (rearOpen) rear->TrainLine = rearP;

            self->TrainLineOpen = !frontOpen || !rearOpen;
        }
    }

    void equalizeBrakeLinePressure(float dT) {
        const float rate = 100.f;
        const float closeRate = 0.08f;

        for (size_t i = 0; i < cars.size(); i++) {
            Ent_Train* self = cars[i].get();
            Ent_Train* front;
            Ent_Train* rear;
            getCoupledNeighbors(i, &front, &rear);

            bool frontOpen = self->brakeValveFront && front != nullptr;
            bool rearOpen = self->brakeValveRear && rear != nullptr;

            double frontP = frontOpen ? front->BrakeLine : 0.0;
            double rearP = rearOpen ? rear->BrakeLine : 0.0;

            equalizeCoupledPressure(dT, self->BrakeLine,
                frontOpen ? &frontP : nullptr, self->brakeValveFront, rate, closeRate);
            if (frontOpen) front->BrakeLine = frontP;

            equalizeCoupledPressure(dT, self->BrakeLine,
                rearOpen ? &rearP : nullptr, self->brakeValveRear, rate, closeRate);
            if (rearOpen) rear->BrakeLine = rearP;
        }
    }

    void updateMovement(float dt) {
        float totalMass = 0.f;
        float totalForce = 0.f;
        for (auto& t : cars) {
            totalMass += t->mass;
            totalForce += t->netForce;
        }
        if (totalMass <= 0.f) return;

        float sharedAccel = totalForce / totalMass;
        float newSpeed = findActiveHead(cars).speed + sharedAccel * dt;

        for (auto& t : cars) {
            t->accel = sharedAccel;
            t->speed = newSpeed;
        }
    }

    void update(float dT) {
        for (auto& t : cars) {
            t->main750v = 750;
        }
        updateWires();
        updateMovement(dT); 
        writeWagonCount();
        equalizeTrainLinePressure(dT);
        equalizeBrakeLinePressure(dT);
    }


    void addWagon(unique_ptr<Ent_Train> wagon, int pos = INT_MAX) {
        if (cars.empty() or pos > cars.size()) cars.push_back(move(wagon));
        else cars.insert(cars.begin() + pos, move(wagon));
    }
    template <typename TrainType>
    void createConsist(int wagonCount, RenderWindow* window, AssetManager& tm) {
        addWagon(make_unique<TrainType>(0, window, tm, true));
        for (int i = 1; i < wagonCount; i++) {
            addWagon(make_unique<TrainType>(i, window, tm));
        }
    }
};

void renderingThread(RenderWindow* window,
    Consist& consist,
    tunnelSet& tunnels,
    atomic<bool>& running,
    tgui::Gui& gui,
    Console& console,
    atomic<bool>& renderFailed
)
{
    Font font;
    font.openFromFile("fonts\\consolas.ttf");
    Text fpsCounter{ font };
    Text simQualityCounter{ font };
    fpsCounter.setCharacterSize(24);
    fpsCounter.setFillColor(sf::Color::White);
    fpsCounter.setPosition(Vector2f(10.f, 10.f));
    simQualityCounter.setCharacterSize(24);
    simQualityCounter.setFillColor(sf::Color::White);
    simQualityCounter.setPosition(Vector2f(10.f, 40.f));
    window->setVerticalSyncEnabled(true);
    try {
        window->clear({ 0,0,0 });
        window->display();

        window->setActive(true);
        HWND hwnd = window->getNativeHandle();
        int   counter = 0;
        float fps = 0.f;
        auto  lastFpsTime = chrono::steady_clock::now();
        while (running.load() && window->isOpen()) {
            auto frameStart = chrono::steady_clock::now();
            window->clear();
            tunnels.draw();
            for (auto& t : consist.cars) {
                t->draw();
            }
            for (auto& t : consist.cars) {
                t->drawui();
            }

            if (GetAsyncKeyState(VK_LWIN) & 0x8000 || GetAsyncKeyState(VK_RWIN) & 0x8000)
                ShowWindow(hwnd, SW_MINIMIZE);

            gui.draw();
            auto dt = chrono::duration_cast<chrono::seconds>(frameStart - lastFpsTime);
            if (dt.count() >= 1) {
                fps = counter / (double)dt.count();
                counter = 0;
                lastFpsTime = frameStart;
            }
            if (showfps) {
                fpsCounter.setString("FPS: " + to_string((int)fps));
                window->draw(fpsCounter);
            }
            if (simQuality) {
                simQualityCounter.setString("SQ: " + to_string(simSpeed));
                window->draw(simQualityCounter);
            }


            window->display();
            ++counter;
        }
    }

    catch (const exception& e) {
        cerr << "Error when rendering: " << e.what() << "\n";
        window->setActive(false);
        renderFailed.store(true);
    }

}
void simulator(
    Consist& consist,
    tunnelSet& tunnels,
    MEventBus& bus,
    atomic<bool>& running,
    tgui::Gui& gui,
    Console& console)
{
    try {

        vector<MEvent> inputEvents;
        inputEvents.reserve(64);

        auto lastTime = chrono::steady_clock::now();

        while (running.load()) {
            auto currentTime = chrono::steady_clock::now();
            chrono::duration<float> deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            float dt = deltaTime.count();

            inputEvents.clear();
            bus.drainTo(inputEvents);

            {
                //cant remember this bullshit, remake from scratch?

                /*auto view = reg.view<train_base::EventBuffer>();
                for (auto e : view) {
                    auto& buffer = view.get<train_base::EventBuffer>(e);
                    for (auto& m : buffer.events) {
                        inputEvents.push_back(m);
                    }
                    buffer.events.clear();
                }*/
            }
            consist.update(dt);

            for (auto& t : consist.cars) {
                t->sim(&inputEvents, (double)dt);
            }

            simSpeed = dt;
            tunnels.simulate((float)dt * findActiveHead(consist.cars).speed * METER_TO_PX);

            auto target = currentTime + chrono::milliseconds(10);
            while (chrono::steady_clock::now() < target) {
                this_thread::yield();
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error in simulation: " << e.what() << "\n";
        console.on();
    }
}
int main()
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    ShowWindow(GetConsoleWindow(), 0);
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "ASMS", sf::State::Fullscreen, settings);
    tgui::Gui gui{ window };
    Console console{ gui };

    ConsoleBuf coutBuf(console, std::cout);
    ConsoleBuf cerrBuf(console, std::cerr, true);
    ConsoleBuf clogBuf(console, std::clog);

    try {

        //setupDebugBox(gui);

        AssetManager tm;
        tunnelSet tunnels(&window, tm.get<Texture>("textures\\tunnels\\tunel_sq_t1.png"), { 0, 345 });
        Consist consist;

        consist.createConsist<Ent_Train_E>(2, &window, tm);


        for (auto& t : consist.cars) {
            t->setScale();
        }
        for (int i = 0; i < consist.cars.size(); i++) {
            sf::Sprite sprite = consist.cars[i]->getSprite(0);
            float posy = (window.getSize().y / 2 - sprite.getGlobalBounds().size.y / 2);
            float posx = (float)i * sprite.getGlobalBounds().size.x - 130 * i;
            Vector2f pos(posx, posy);
            consist.cars[i]->setPos(pos);
            consist.cars[i]->updatePos();
        }

        MEventBus bus;
        atomic<bool> running{ true };
        atomic<bool> renderFailed{ false };

        window.setActive(false);
        jthread renderThread(renderingThread, &window, ref(consist), ref(tunnels), ref(running), ref(gui), ref(console), ref(renderFailed));
        jthread simThread([&]() { simulator(consist, tunnels, bus, running, gui, console); });


        while (window.isOpen()) {
            if (renderFailed.load()) {
                failureDraw(console, gui, &window);
            }
            while (const optional ev = window.pollEvent()) {
                gui.handleEvent(*ev);
                if (ev->is<Event::Closed>()) {
                    running.store(false);
                    if (renderThread.joinable()) renderThread.join();
                    if (simThread.joinable())    simThread.join();
                    window.close();
                }
                else if (ev->is<Event::MouseButtonPressed>() ||
                    ev->is<Event::MouseButtonReleased>())
                {
                    for (auto& t : consist.cars) t->ui.checkEvents(*ev);
                }
                else if (ev->is<Event::MouseMoved>()) {
                    for (auto& t : consist.cars) t->ui.checkEvents(*ev);
                }
                else if (const auto* kp = ev->getIf<Event::KeyPressed>()) {
                    if (kp->code == sf::Keyboard::Key::Grave)console.toggle();
                    if (!console.isVisible()) {
                        MEvent m;
                        m.type = "KeyPressed";
                        m.sender = "window";
                        m.key = kp->code;
                        m.alt = kp->alt;
                        m.control = kp->control;
                        m.shift = kp->shift;
                        m.system = kp->system;
                        bus.emit(m);
                    }
                }

            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }

        running.store(false);
        if (renderThread.joinable()) renderThread.join();
        if (simThread.joinable()) simThread.join();
        return 0;
    }
    catch (const exception& e) {
        cerr << "Error on startup/event gather: " << e.what() << "\n";
        console.on();
        failureDraw(console, gui, &window);
        return 1;
    }
}