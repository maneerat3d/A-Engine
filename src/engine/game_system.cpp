#include "game_system.h"
#include "core/world/world.h"
#include <SDL.h>
#include <glm/gtc/quaternion.hpp>

namespace AEngine {

void GameSystem::init() {
    // Initialization logic for game systems if needed
}

void GameSystem::update(World& world, float dt) {
    // Logic for rotating cubes, moved from engine.cpp
    float time = (float)SDL_GetTicks() / 1000.0f;
    for (Entity i = 1; i < world.getEntityCount(); ++i) { 

        // ตรวจสอบว่า Entity i มีทั้ง TransformComponent และ RotatingCubeComponent หรือไม่
        // หมายเหตุ: ใน ECS จริงควรมีฟังก์ชัน world.hasComponent<T>(i) แต่ตอนนี้เราใช้วิธีตรวจสอบ Pointer ไปก่อน
        auto* rot_comp_ptr = &world.getComponent<ECS::RotatingCubeComponent>(i);
        auto* transform_ptr = &world.getComponent<ECS::TransformComponent>(i);

        if (rot_comp_ptr && transform_ptr && rot_comp_ptr->rotationSpeed > 0.0f) {
             transform_ptr->rotation = glm::angleAxis(time * rot_comp_ptr->rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));
        }

        // This is still a simplified way to iterate. A real system would be more efficient.
        // For example, it would only iterate over entities that have both components.
        auto& rot_comp = world.getComponent<ECS::RotatingCubeComponent>(i);
        if (rot_comp.rotationSpeed > 0.0f) { // Check if the component is active
             auto& transform = world.getComponent<ECS::TransformComponent>(i); 
             transform.rotation = glm::angleAxis(time * rot_comp.rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));
        }
    }
}

void GameSystem::shutdown() {
    // Shutdown logic for game systems if needed
}

} // namespace AEngine