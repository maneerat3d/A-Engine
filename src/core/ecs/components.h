#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <string>

// Forward declarations
namespace AEngine {
class Mesh;
class Texture;
}

namespace AEngine::ECS {

// Component สำหรับเก็บข้อมูลตำแหน่ง การหมุน และขนาด
struct TransformComponent {
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::quat rotation = {1.0f, 0.0f, 0.0f, 0.0f}; // W, X, Y, Z
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    glm::mat4 getMatrix() const {
        glm::mat4 posMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return posMatrix * rotMatrix * scaleMatrix;
    }
};

// Component สำหรับเก็บข้อมูลที่ต้องใช้ในการเรนเดอร์
struct RenderableComponent {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
};

// Component สำหรับ Script ที่ทำให้วัตถุหมุน (ตัวอย่าง)
struct RotatingCubeComponent {
    float rotationSpeed = 1.0f;
};

} // namespace AEngine::ECS