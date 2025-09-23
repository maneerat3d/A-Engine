#pragma once
#include <cstdint>
// Forward declaration
struct SDL_Window;

namespace AEngine { 
    class World; 
    class Framebuffer;
} 
namespace AEngine {

class Renderer {
public:
    ~Renderer();

    // เราจะส่ง SDL_Window เข้ามาตอน init
    void init(SDL_Window* window);
    void render(World& world, Framebuffer* framebuffer = nullptr);
    void shutdown();
    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

private:
    // Member variables ทั้งหมดที่เกี่ยวกับ OpenGL ถูกย้ายมาที่นี่

    unsigned int m_shader_program; // ยังเก็บ Shader program ไว้ก่อน
    int m_model_loc;
    int m_view_loc;
    int m_projection_loc;

    SDL_Window* m_window; // Renderer ต้องรู้จัก window เพื่อใช้ในฟังก์ชัน swap
};

} // namespace AEngine