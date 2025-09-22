#pragma once

// Forward declaration
struct SDL_Window;

// Forward declare Scene เพื่อไม่ต้อง include scene.h
namespace AEngine { class World; } 

namespace AEngine {

class Renderer {
public:
    Renderer();
    ~Renderer();

    // เราจะส่ง SDL_Window เข้ามาตอน init
    void init(SDL_Window* window);
    void render(World& world); 
    void shutdown();

private:
    // Member variables ทั้งหมดที่เกี่ยวกับ OpenGL ถูกย้ายมาที่นี่

    unsigned int m_shader_program; // ยังเก็บ Shader program ไว้ก่อน
    int m_model_loc;
    int m_view_loc;
    int m_projection_loc;

    SDL_Window* m_window; // Renderer ต้องรู้จัก window เพื่อใช้ในฟังก์ชัน swap
};

} // namespace AEngine