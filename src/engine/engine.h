#pragma once

// Forward declaration เพื่อไม่ต้อง include SDL.h ใน header
struct SDL_Window;
using SDL_GLContext = void*; // Forward declare GL Context

// Forward declare Renderer เพื่อไม่ต้อง include renderer.h
namespace AEngine {
class Renderer;
class Scene; 
}

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

    // Member variables สำหรับเก็บสถานะของ Engine
    SDL_Window* m_window; // Engine ยังคงเป็นเจ้าของ window
    SDL_GLContext m_gl_context; // และ GL Context
    bool m_is_running;
    Renderer* m_renderer; // Engine มี pointer ไปยัง Renderer
    Scene* m_scene;       // Engine เป็นเจ้าของ Scene
};

} // namespace AEngine