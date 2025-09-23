#include "render_system.h"
#include "renderer.h"
#include "framebuffer.h"
#include "engine/engine.h"
#include "platform/platform_subsystem.h"
#include "core/world/world.h"
#include "core/memory/core.h"


namespace AEngine {

RenderSystem::RenderSystem(Engine& engine)
    : m_renderer(nullptr), m_engine(engine) {
}

RenderSystem::~RenderSystem() {
    // Destructor จะถูกเรียกตอน shutdown, m_renderer จะถูกลบใน shutdown()
}

void RenderSystem::renderToFramebuffer(World& world, Framebuffer* framebuffer) {
    // ส่ง world และ framebuffer ต่อไปให้ renderer จัดการ
    if (m_renderer) {
        m_renderer->render(world, framebuffer);
    }
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