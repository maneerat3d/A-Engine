#pragma once

// Forward declaration
struct SDL_Window;

// Forward declare Scene เพื่อไม่ต้อง include scene.h
namespace AEngine {
class Scene;
}

namespace AEngine {

class Renderer {
public:
    Renderer();
    ~Renderer();

    // เราจะส่ง SDL_Window เข้ามาตอน init
    void init(SDL_Window* window);
    void render(Scene& scene);
    void shutdown();

private:
    // Member variables ทั้งหมดที่เกี่ยวกับ OpenGL ถูกย้ายมาที่นี่
    unsigned int m_shader_program;
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_texture;

    int m_model_loc;
    int m_view_loc;
    int m_projection_loc;

    SDL_Window* m_window; // Renderer ต้องรู้จัก window เพื่อใช้ในฟังก์ชัน swap
};

} // namespace AEngine