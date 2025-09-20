#pragma once

// Forward declaration เพื่อไม่ต้อง include SDL.h ใน header
struct SDL_Window;
using SDL_GLContext = void*; // Forward declare GL Context

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
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
     bool m_is_running;
 
    // Member variables สำหรับเก็บ OpenGL objects
    unsigned int m_shader_program;
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_texture;

    // Member variables สำหรับเก็บ location ของ uniform
    int m_model_loc;
    int m_view_loc;
    int m_projection_loc;
};

} // namespace AEngine