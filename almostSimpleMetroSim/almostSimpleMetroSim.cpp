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
#define METER_TO_PX 71.1f
#include "AssetManager.hpp"
#include "MGraphics.hpp"
#include "mevent.hpp"
#include "tunnel.hpp"
#include "train_base.hpp"
#include "703_E.hpp"
#include "includes.h"
bool showfps = false;
bool simQuality = false;
#include "console.hpp"
#include <SFML/OpenGL.hpp>
#include <consoleapi3.h>
#include <windows.h>

using namespace std;
using namespace sf;

//#define KOSTIL

float simSpeed = 1.f;

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
    void writeWagonCount(entt::registry& reg) {
        int count = (int)order.size();
        for (entt::entity e : order) {
            reg.get<train_base::WagonCount>(e).count = count;
        }
	}
    void syncSpeed(entt::registry& reg) {
		float speed = reg.get<train_base::Movement>(head()).speed;
        for (entt::entity e : order) {
            auto& mv = reg.get<train_base::Movement>(e);
            mv.speed = speed;
        }
	}
    void update(entt::registry& reg) {
		updateWires(reg);
		syncSpeed(reg);
        writeWagonCount(reg);
	}

    void addWagon(entt::registry& reg, entt::entity wagon) {
        order.push_back(wagon);
	}

    void createConsist(entt::registry& reg, int wagonCount, RenderWindow* window, AssetManager& tm) {
        if (wagonCount <= 0) return;
        entt::entity head = makeWagonE(reg, window, tm, true);
        addWagon(reg, head);
        for (int i = 1; i < wagonCount; i++) {
            entt::entity wagon = makeWagonE(reg, window, tm);
			addWagon(reg, wagon);
		}
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
            window->clear();
            tunnels.draw();
            train_base_systems::drawAll(reg, *window);
            train_base_systems::drawUI(reg, *window);

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
            if(simQuality) {
                simQualityCounter.setString("SQ: " + to_string(simSpeed));
                window->draw(simQualityCounter);
			}
            

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
        auto lastTime = chrono::steady_clock::now();

        while (running.load()) {
            auto currentTime = chrono::steady_clock::now();
            chrono::duration<float> deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            float dt = deltaTime.count();

            inputEvents.clear();
            bus.drainTo(inputEvents);

            {
                auto view = reg.view<train_base::EventBuffer>();
                for (auto e : view) {
                    auto& buffer = view.get<train_base::EventBuffer>(e);
                    for (auto& m : buffer.events) {
                        inputEvents.push_back(m);
                    }
                    buffer.events.clear();
                }
            }
			consist.update(reg);

            train_e_systems::simAllWagonsE(reg, inputEvents, (double)dt);
            simSpeed = dt;
            if (first) {
                tunnels.generateTunnel();
                tunnels.moveTunnels(Vector2f(1500.f, 0.f));
                first = false;
            }
            else {
                tunnels.generateTunnel();
            }

            float speed = train_base_systems::readSpeed(reg, consist.head());

            tunnels.moveTunnels(Vector2f(speed * dt * METER_TO_PX, 0.f));

            auto frameEnd = chrono::steady_clock::now();
            auto processTime = frameEnd - currentTime;

            if (processTime < chrono::milliseconds(10)) {
                this_thread::sleep_for(chrono::milliseconds(10) - processTime);
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
    ShowWindow(GetConsoleWindow(), 0);
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
#ifdef KOSTIL
    sf::RenderWindow window(desktop, "ASMS", sf::State::Windowed, settings);
	window.setPosition({ 0, 0 });
#else
	sf::RenderWindow window(desktop, "ASMS", sf::State::Fullscreen, settings);
#endif
    tgui::Gui gui{ window };
    Console console{ gui };

    ConsoleBuf coutBuf(console, std::cout);
    ConsoleBuf cerrBuf(console, std::cerr, true);
    ConsoleBuf clogBuf(console, std::clog);

    try {
        
        AssetManager tm;
        tunnelSet tunnels(&window, tm.get<Texture>("textures\\tunnels\\tunel_sq_t1.png"), { 0, 345 });
        entt::registry reg;
        Consist consist;

		consist.createConsist(reg, 2, &window, tm);
        
        train_base_systems::applyScale(reg);

        
        for (int i = 0; i < (int)consist.order.size(); i++) {
            entt::entity e = consist.order[i];
            auto& spl = reg.get<train_base::SpriteList>(e);
            auto& rp = reg.get<train_base::RelPos>(e);

            
            const Sprite& body = spl.sprites[train_e::SpriteSlot::Body].sprite;
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