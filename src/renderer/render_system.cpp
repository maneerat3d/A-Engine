#include "render_system.h"
#include "renderer.h"
#include "engine/engine.h"
#include "platform/platform_subsystem.h"
#include "core/world/world.h"
#include "core/memory/core.h" // <--- เพิ่มบรรทัดนี้

// -- PHASE 1 (ต่อ): แก้ไขการเชื่อมต่อของ RenderSystem --
// - Constructor ถูกแก้ไขให้รับและเก็บ Engine context
// - init() จะขอ SDL_Window จาก PlatformSubsystem ผ่าน Engine
// - update() รับ World& เข้ามาและส่งต่อไปให้ Renderer

namespace AEngine {

RenderSystem::RenderSystem(Engine& engine)
    : m_renderer(nullptr), m_engine(engine) {
}

RenderSystem::~RenderSystem() {
    // Destructor จะถูกเรียกตอน shutdown, m_renderer จะถูกลบใน shutdown()
}

void RenderSystem::init() {
    // ใช้ Allocator ของ Engine ในการสร้าง Renderer
    m_renderer = AENGINE_NEW(m_engine.getAllocator(), Renderer)();

    // ขอ Window จาก PlatformSubsystem ผ่าน Engine context
    PlatformSubsystem* platform = m_engine.getSubsystem<PlatformSubsystem>();
    if (platform && platform->getWindow()) {
        m_renderer->init(platform->getWindow());
    }
}

void RenderSystem::update(World& world, float dt) {
    // ส่ง world ทั้งก้อนต่อไปให้ renderer จัดการ
    if (m_renderer) {
        m_renderer->render(world);
    }
}

void RenderSystem::shutdown() {
    if (m_renderer) {
        m_renderer->shutdown();
        // ใช้ Allocator ของ Engine ในการลบ Renderer
        AENGINE_DELETE(m_engine.getAllocator(), m_renderer);
        m_renderer = nullptr;
    }
}

} // namespace AEngine