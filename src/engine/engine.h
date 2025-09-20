#pragma once

// Forward declaration เพื่อไม่ต้อง include SDL.h ใน header
struct SDL_Window;

namespace AEngine {

class Engine {
public:
    Engine();
    ~Engine();

    void run();

private:
    void init();
    void gameLoop();
    void shutdown();

    SDL_Window* m_window;
    bool m_is_running;
};

} // namespace AEngine