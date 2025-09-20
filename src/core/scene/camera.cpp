#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace AEngine {

Camera::Camera()
    : m_position(0.0f, 0.0f, 3.0f)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
{
    updateViewMatrix();
    m_projectionMatrix = glm::mat4(1.0f);
}

void Camera::setPerspective(float fov, float aspect, float near, float far) {
    m_projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::setPosition(const glm::vec3& position) {
    m_position = position;
    updateViewMatrix();
}

void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
    m_front = glm::normalize(target - m_position);
    m_up = up;
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

} // namespace AEngine