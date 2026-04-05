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
#include <entt/entt.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "TextureManager.hpp"
#include "MGraphics.hpp"
#include "mevent.hpp"
#include "tunnel.hpp"
#include "train_base.hpp"
#include "703_E.hpp"
#include "includes.h"
#include "console.hpp"
#include <SFML/OpenGL.hpp>
#include <consoleapi3.h>
#include <windows.h>

using namespace std;
using namespace sf;

//sf::RenderWindow window;
void crash() {
    std::vector<int> v;
    int i = v[0xfffffffffffffff2];
}
void failureDraw(Console&console, tgui::Gui& gui, RenderWindow*window) {
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
struct Consist {
    vector<entt::entity> order;

    entt::entity head() const { return order.front(); }

    void updateWires(entt::registry& reg) {
        train_base_systems::updateWires(reg, order);
    }
}; 

void renderingThread(RenderWindow* window,
    entt::registry& reg,
    tunnelSet& tunnels,
    atomic<bool>& running,
    tgui::Gui& gui,
    Console& console,
	atomic<bool>& renderFailed
)
{
    try {
        window->clear({ 0,0,0 });
        window->display();

        window->setActive(true);
        HWND hwnd = window->getNativeHandle();
        int   counter = 0;
        float fps = 0.f;
        auto  lastFpsTime = chrono::steady_clock::now();
        const chrono::duration<double> targetFrame(1.0 / 144.0);
		//throw runtime_error("Simulated render failure");
        while (running.load() && window->isOpen()) {
            auto frameStart = chrono::steady_clock::now();

            // FPS counter
            auto dt = chrono::duration_cast<chrono::seconds>(frameStart - lastFpsTime);
            if (dt.count() >= 1) {
                fps = counter / (double)dt.count();
                counter = 0;
                lastFpsTime = frameStart;
                cout << "FPS: " << fps << "\n";
            }

            window->clear();
            tunnels.draw();
            train_base_systems::drawAll(reg, *window);
            train_base_systems::drawUI(reg, *window);

            if (GetAsyncKeyState(VK_LWIN) & 0x8000 || GetAsyncKeyState(VK_RWIN) & 0x8000)
                ShowWindow(hwnd, SW_MINIMIZE);

            gui.draw();

            window->display();
            ++counter;
            auto elapsed = chrono::steady_clock::now() - frameStart;
            if (elapsed < targetFrame)
                this_thread::sleep_for(targetFrame - elapsed);
        }
    }
    catch (const exception& e) {
        cerr << "Error when rendering: " << e.what() << "\n";
        window->setActive(false);
        renderFailed.store(true);
    }

}
void simulator(entt::registry& reg,
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
		bool first = true;
        tunnels.generateTunnel();
       

        while (running.load()) {
            auto start = chrono::steady_clock::now();


            inputEvents.clear();
            bus.drainTo(inputEvents);


            {
                auto view = reg.view<train_base::EventBuffer>();
                for (auto e : view)
                    for (auto& m : view.get<train_base::EventBuffer>(e).events)
                        inputEvents.push_back(m);
            }


            consist.updateWires(reg);
            train_e_systems::simAllWagonsE(reg, inputEvents, 0.01f);

            if (first) {
                tunnels.generateTunnel();
                tunnels.moveTunnels(Vector2f(1500, 0.f));
                first = false;
            }
            else {
                tunnels.generateTunnel();
            }
            float moved = train_base_systems::readSpeed(reg, consist.head());
            tunnels.moveTunnels(Vector2f(moved, 0));


            const auto frameTime = chrono::duration<double>(0.01);
            auto elapsed = chrono::steady_clock::now() - start;
            if (elapsed < frameTime)
                this_thread::sleep_for(frameTime - elapsed);
        }
    }
    catch (const exception& e) {
        cerr << "Error in simulation: " << e.what() << "\n";
		console.on();
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), 0);
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "ASMS", sf::State::Fullscreen, settings);

    tgui::Gui gui{ window };
    Console console{ gui };

    ConsoleBuf coutBuf(console, std::cout);
    ConsoleBuf cerrBuf(console, std::cerr);
    ConsoleBuf clogBuf(console, std::clog);

    try {
        
        TextureManager tm;
        tunnelSet tunnels(&window, tm.get("textures\\tunnels\\tunel_sq_t1.png"), { 0, 345 });
        entt::registry reg;
        Consist consist;
        consist.order.push_back(makeWagonE(reg, &window, tm, true));
        consist.order.push_back(makeWagonE(reg, &window, tm, false));

        
        train_base_systems::applyScale(reg);

        
        for (int i = 0; i < (int)consist.order.size(); i++) {
            entt::entity e = consist.order[i];
            auto& spl = reg.get<train_base::SpriteList>(e);
            auto& rp = reg.get<train_base::RelPos>(e);

            
            const Sprite& body = spl.sprites[5].sprite;
            float posy = window.getSize().y / 2.f - body.getGlobalBounds().size.y / 2.f;
            float posx = i * body.getGlobalBounds().size.x - 130.f * i;
            rp.pos = { posx, posy };
            spl.updatePositions(rp.pos);
        }

        MEventBus bus;
        atomic<bool> running{ true };
		atomic<bool> renderFailed{ false };

        window.setActive(false);
        jthread renderThread(renderingThread, &window, ref(reg), ref(tunnels), ref(running), ref(gui), ref(console), ref(renderFailed));
        jthread simThread([&]() { simulator(reg, consist, tunnels, bus, running, gui, console); });

        
        while (window.isOpen()) {
            if (renderFailed.load()) {
                failureDraw( console, gui,&window);
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
                    train_base_systems::checkUIEvents(reg, *ev);
                }
                else if (ev->is<Event::MouseMoved>()) {
                    train_base_systems::checkUIEvents(reg, *ev);
                }
                else if (const auto* kp = ev->getIf<Event::KeyPressed>()) {
                    if (kp->code == sf::Keyboard::Key::Grave)
                        console.toggle();
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
            this_thread::sleep_for(chrono::nanoseconds(100));
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