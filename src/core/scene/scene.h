#pragma once

#include "camera.h"
#include "ecs/ecs.h"
#include "ecs/components.h"
#include <array>

namespace AEngine {

class Scene {
public:
    Scene();
    ~Scene();

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
inline std::array<ECS::TransformComponent, 1024>& Scene::getComponentArray<ECS::TransformComponent>() {
    return m_transformComponents;
}

template<>
inline std::array<ECS::RenderableComponent, 1024>& Scene::getComponentArray<ECS::RenderableComponent>() {
    return m_renderableComponents;
}

template<>
inline std::array<ECS::RotatingCubeComponent, 1024>& Scene::getComponentArray<ECS::RotatingCubeComponent>() {
    return m_rotatingCubeComponents;
}

} // namespace AEngine