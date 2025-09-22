#include "game_plugin.h"
#include "engine/engine.h"
#include "game_system.h"
// เพิ่ม include ที่จำเป็น
#include "core/world/world_manager.h" // <--- ตัวใหม่
#include "core/world/world.h"         // <--- ตัวใหม่
#include "core/resource/resource_manager.h"
#include "renderer/mesh.h"
#include <iostream>

namespace AEngine {

GamePlugin::GamePlugin(Engine& engine) : m_engine(engine) {
    std::cout << "Game Plugin Created" << std::endl;
}

GamePlugin::~GamePlugin() {
    std::cout << "Game Plugin Destroyed" << std::endl;
}

void GamePlugin::createSystems(Engine& engine) {
    // 1. เพิ่ม GameSystem (ยังเหมือนเดิม)
    engine.addSystem(new GameSystem());

    // 2. โหลดโมเดลและสร้าง Entities ผ่าน Subsystem
    WorldManager* worldManager = engine.getSubsystem<WorldManager>();
    ResourceManager* resourceManager = engine.getSubsystem<ResourceManager>();

    if (worldManager && resourceManager) {
        World* world = worldManager->getActiveWorld(); // <--- เอา World มาจาก WorldManager
        if (!world) return;

        // การโหลด Resource ยังคงเหมือนเดิม แต่เราได้ ResourceManager มาจากวิธีใหม่
        auto mainMeshResource = resourceManager->load<Mesh>("models/monkey.glb");
        auto allMonkeyMeshes = resourceManager->findAll<Mesh>("models/monkey.glb");

        if (!allMonkeyMeshes.empty()) {
            Entity model1 = world->createEntity(); // <--- สร้าง Entity จาก world
            {
                ECS::TransformComponent transform;
                transform.position = {-1.5f, 0.0f, 0.0f};
                world->addComponent(model1, transform); // <--- เพิ่ม Component ให้ world
                world->addComponent<ECS::RenderableComponent>(model1, {allMonkeyMeshes[0], allMonkeyMeshes[0]->getTexture()});
                world->addComponent<ECS::RotatingCubeComponent>(model1, {1.0f});
            }
        }
    }
}

void GamePlugin::destroySystems(Engine& engine) {
    // Systems are owned and destroyed by the engine
}

} // namespace AEngine