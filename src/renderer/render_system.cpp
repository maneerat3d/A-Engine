#include "render_system.h"
#include "renderer.h"
#include "framebuffer.h"
#include "engine/engine.h"
#include "platform/platform_subsystem.h"
#include "core/world/world.h"
#include "core/memory/core.h"
#include "camera.h"

namespace AEngine {

RenderSystem::RenderSystem(Engine& engine)
    : m_renderer(nullptr), m_engine(engine) {
}
void RenderSystem::onViewportResize(uint32_t width, uint32_t height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

RenderSystem::~RenderSystem() {
    // Destructor จะถูกเรียกตอน shutdown, m_renderer จะถูกลบใน shutdown()
}

void RenderSystem::renderToFramebuffer(World& world, Framebuffer* framebuffer) {
    if (m_renderer && m_viewportWidth > 0 && m_viewportHeight > 0) {
        // 1. อัปเดต Aspect Ratio ของกล้องให้ถูกต้องก่อนเรนเดอร์
        //    นี่คือหัวใจของการแก้ปัญหาภาพยืด
        Camera& camera = world.getCamera(); // สมมติว่า World มีฟังก์ชัน getCamera()
        camera.setAspectRatio((float)m_viewportWidth / (float)m_viewportHeight);

        // 2. ตั้งค่า Viewport ของ Renderer ให้ตรงกับขนาดของ Framebuffer
        m_renderer->setViewport(0, 0, m_viewportWidth, m_viewportHeight);

        // 3. ส่งต่อให้ Renderer วาดตามปกติ
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