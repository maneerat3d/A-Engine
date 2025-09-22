#pragma once
// -- PHASE 1: Decoupling the Core Engine --
// Step 1.3: Elevate ResourceManager and World
// เปลี่ยนชื่อ Scene เป็น World เพื่อให้สื่อความหมายว่าเป็น Container ของทุกสิ่งในเกม
// โครงสร้างภายในยังเหมือนเดิม แต่จะถูกจัดการโดย WorldManager แทนที่จะเป็น Engine โดยตรง
#include "core/scene/camera.h"
#include "core/ecs/ecs.h"
#include "core/ecs/components.h"
#include <array>

namespace AEngine {

class World { 
public:
    World();
    ~World();

    Entity createEntity();

    template<typename T>
    void addComponent(Entity entity, T component) {
        // เรียกใช้ private member function template ที่ถูกต้อง
        getComponentArray<T>()[entity] = component;
    }

    template<typename T>
    T& getComponent(Entity entity) {
        return getComponentArray<T>()[entity];
    }
    
    Camera& getCamera() { return m_camera; }
    Entity getEntityCount() const { return m_entityCounter; }

private:
    // 1. ประกาศ member function template ไว้ภายในคลาส
    template<typename T>
    std::array<T, 1024>& getComponentArray();

    Camera m_camera;
    Entity m_entityCounter = 1; // Start from 1, 0 is invalid
    
    // ย้าย Component arrays มาไว้ใน private section
    std::array<ECS::TransformComponent, 1024> m_transformComponents;
    std::array<ECS::RenderableComponent, 1024> m_renderableComponents;
    std::array<ECS::RotatingCubeComponent, 1024> m_rotatingCubeComponents;
};

// 2. ทำ Template Specialization นอกคลาส แต่ยังอยู่ในไฟล์ .h (เพื่อให้ inline ได้)
//    โดยอ้างอิงว่าเป็น member ของ Scene (Scene::) อย่างถูกต้อง
template<>
inline std::array<ECS::TransformComponent, 1024>& World::getComponentArray<ECS::TransformComponent>() {
    return m_transformComponents;
}

template<>
inline std::array<ECS::RenderableComponent, 1024>& World::getComponentArray<ECS::RenderableComponent>() {
    return m_renderableComponents;
}

template<>
inline std::array<ECS::RotatingCubeComponent, 1024>& World::getComponentArray<ECS::RotatingCubeComponent>() {
    return m_rotatingCubeComponents;
}

} // namespace AEngine