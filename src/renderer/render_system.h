#pragma once
#include "core/ecs/isystem.h"
#include <cstdint>

// Forward declarations
namespace AEngine {
class Renderer;
class Framebuffer;
class Engine;
class World; // ใช้ World แทน Scene

class RenderSystem : public ISystem {
public:
    // Constructor จะรับ Engine context เข้ามาแทน SDL_Window*
    explicit RenderSystem(Engine& engine);
    ~RenderSystem();

    // --- ISystem Interface Implementation ---
    void init() override;
    void update(World& world, float dt) override;
    void renderToFramebuffer(World& world, Framebuffer* framebuffer);
    void shutdown() override;

    void onViewportResize(uint32_t width, uint32_t height);

private:
    Renderer* m_renderer;
    Engine& m_engine; // เก็บ reference ไปยัง Engine เพื่อใช้ getSubsystem

    uint32_t m_viewportWidth = 1280;
    uint32_t m_viewportHeight = 720;
};

} // namespace AEngine