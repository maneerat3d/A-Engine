#pragma once
#include "core/ecs/isystem.h"

// -- PHASE 1 (ต่อ): แก้ไขการเชื่อมต่อของ RenderSystem --
// - ทำให้ RenderSystem รู้จัก Engine เพื่อดึง Subsystem ที่จำเป็น (เช่น PlatformSubsystem)
// - เปลี่ยนจากการรับ Scene มาเป็น World ให้สอดคล้องกับโครงสร้างใหม่

// Forward declarations
namespace AEngine {
class Renderer;
class Engine;
class World; // ใช้ World แทน Scene

class RenderSystem : public ISystem {
public:
    // Constructor จะรับ Engine context เข้ามาแทน SDL_Window*
    explicit RenderSystem(Engine& engine);
    ~RenderSystem();

    // --- ISystem Interface Implementation ---
    void init() override;
    void update(World& world, float dt) override; // แก้ไขให้รับ World&
    void shutdown() override;

private:
    Renderer* m_renderer;
    Engine& m_engine; // เก็บ reference ไปยัง Engine เพื่อใช้ getSubsystem
};

} // namespace AEngine