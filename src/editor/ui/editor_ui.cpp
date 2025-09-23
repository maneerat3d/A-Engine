#include "editor_ui.h"
#include "editor/editor.h"
#include "engine/engine.h"
#include "core/resource/resource_manager.h"
#include "core/world/world_manager.h"
#include "core/world/world.h"
#include "core/ecs/components.h"
#include "renderer/mesh.h"
#include "renderer/texture.h"
#include "portable-file-dialogs.h"

#include <imgui.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace AEngine {

EditorUI::EditorUI(Editor& editor, Engine& engine, IAllocator& allocator)
    : m_editor(editor)
    , m_engine(engine)
    , m_allocator(allocator)
{
}

EditorUI::~EditorUI() {
}

// --- ส่วนที่แก้ไข: แก้ไข onGUI() เป็น drawMenuBarItems() และลบ BeginMenuBar() ออก ---
void EditorUI::drawMenuBarItems() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load Object...")) {
            std::cout << "[EditorUI] 'Load Object...' menu item clicked." << std::endl;
            // --- ส่วนที่เปลี่ยน ---
            auto selection = pfd::open_file("Load Object", "models",
                                             { "GLTF Models", "*.gltf *.glb", "All Files", "*" },
                                             pfd::opt::none).result();

            if (!selection.empty()) {
                drawLoadObjectDialog(selection[0]);
            }
        }
        if (ImGui::MenuItem("Exit")) {
            // m_editor.requestExit(); 
        }
        ImGui::EndMenu();
    }
}

// --- ส่วนที่แก้ไข: เพิ่มฟังก์ชัน drawPopups() ---
void EditorUI::drawPopups() {
    
}


void EditorUI::drawLoadObjectDialog(const std::string& selected_path) {
    ResourceManager* resourceManager = m_engine.getSubsystem<ResourceManager>();
    WorldManager* worldManager = m_engine.getSubsystem<WorldManager>();

    if (resourceManager && worldManager) {
        World* world = worldManager->getActiveWorld();
        if (world) {
            auto mesh = resourceManager->load<Mesh>(selected_path);
            if (mesh) {
                Entity new_entity = world->createEntity();

                ECS::TransformComponent transform;
                transform.position = {0.0f, 0.0f, 0.0f};
                world->addComponent(new_entity, transform);

                world->addComponent<ECS::RenderableComponent>(new_entity, {mesh, mesh->getTexture()});
                world->addComponent<ECS::RotatingCubeComponent>(new_entity, {1.0f});

                std::cout << "EditorUI: Loaded object '" << selected_path << "' and created entity " << new_entity << std::endl;
            } else {
                std::cerr << "EditorUI: Failed to load mesh from '" << selected_path << "'" << std::endl;
            }
        }
    }
}

} // namespace AEngine