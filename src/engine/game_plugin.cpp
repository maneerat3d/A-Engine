#include "game_plugin.h"
#include "engine/engine.h"
#include "game_system.h"
// เพิ่ม include ที่จำเป็นสำหรับการสร้าง Entity
#include "core/scene/scene.h"
#include "core/resource/resource_manager.h"
#include "renderer/mesh.h" // ต้องรู้จัก Mesh
#include <iostream>

namespace AEngine {

GamePlugin::GamePlugin(Engine& engine) : m_engine(engine) {
    std::cout << "Game Plugin Created" << std::endl;
}

GamePlugin::~GamePlugin() {
    std::cout << "Game Plugin Destroyed" << std::endl;
}

void GamePlugin::createSystems(Engine& engine) {
    // 1. เพิ่ม GameSystem เข้าไปใน Engine
    engine.addSystem(new GameSystem());
    // 2. โหลดโมเดลและสร้าง Entities เริ่มต้น
    Scene* scene = engine.getScene(); // ต้องเพิ่ม getScene() ใน Engine.h
    ResourceManager* resourceManager = engine.getResourceManager();
    
    if (scene && resourceManager) {
        // ใช้ ResourceManager โหลดโมเดล (ตอนนี้ต้องใช้ load<Mesh> ไปก่อน เพราะ Importer ยังไม่ได้คืนค่ามาโดยตรง)
        // ในอนาคตอาจปรับปรุง ResourceManager ให้ดีกว่านี้
        // 1. สั่งให้ ResourceManager โหลดโมเดล
        // บรรทัดนี้จะไปเรียก GltfImporter โดยอัตโนมัติ และ Importer จะสร้าง Mesh object เก็บไว้ใน ResourceManager
        auto mainMeshResource = resourceManager->load<Mesh>("models/monkey.glb");
        // 2. ดึง Mesh ทั้งหมดที่ถูกสร้างจากไฟล์ "models/monkey.glb" ออกมา
        auto allMonkeyMeshes = resourceManager->findAll<Mesh>("models/monkey.glb");


        if (!allMonkeyMeshes.empty()) {
            Entity model1 = scene->createEntity();
            {
                ECS::TransformComponent transform;
                transform.position = {-1.5f, 0.0f, 0.0f};
                scene->addComponent(model1, transform);
                scene->addComponent<ECS::RenderableComponent>(model1, {allMonkeyMeshes[0], allMonkeyMeshes[0]->getTexture()});
                scene->addComponent<ECS::RotatingCubeComponent>(model1, {1.0f});
            }

            // คุณสามารถสร้าง Entity อื่นๆ ต่อได้ตามต้องการ
        }
    }    
}

void GamePlugin::destroySystems(Engine& engine) {
    // Systems are owned and destroyed by the engine
}

} // namespace AEngine