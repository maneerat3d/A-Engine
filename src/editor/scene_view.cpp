#include "scene_view.h"
#include "renderer/framebuffer.h"
#include "engine/engine.h"
#include "core/world/world_manager.h"
#include "core/world/world.h"
#include "renderer/render_system.h"
#include "core/ecs/isystem.h"

#include <imgui.h>

namespace AEngine {

SceneView::SceneView(Engine* engine)
    : m_engine(engine) {
    m_framebuffer = UniquePtr<Framebuffer>::create(engine->getAllocator(), 1280, 720);
}

SceneView::~SceneView() {
}

void SceneView::onGUI() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Scene View");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

       // เพิ่มการตรวจสอบว่าขนาดมากกว่า 0 เพื่อป้องกัน error ตอนย่อหน้าต่างสุดๆ
    if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0 &&
        (m_width != (u32)viewportPanelSize.x || m_height != (u32)viewportPanelSize.y))
    {
        m_width = (u32)viewportPanelSize.x;
        m_height = (u32)viewportPanelSize.y;
        m_framebuffer->resize(m_width, m_height);

		RenderSystem* renderSystem = m_engine->getSystem<RenderSystem>();

        if (renderSystem) {
			renderSystem->onViewportResize(m_width, m_height);
		}
    }

    // Render world to framebuffer
    WorldManager* worldManager = m_engine->getSubsystem<WorldManager>();
    
    if (worldManager) {
        World* world = worldManager->getActiveWorld();
        if (world) {

            RenderSystem* renderSystem = m_engine->getSystem<RenderSystem>();

            if (renderSystem) {
                renderSystem->renderToFramebuffer(*world, m_framebuffer.get());
            }
        }
    }

    // Display framebuffer texture
    uint64_t textureID = m_framebuffer->getColorAttachmentID();
    ImGui::Image((void*)textureID, ImVec2{ (float)m_width, (float)m_height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar();
}

} // namespace AEngine