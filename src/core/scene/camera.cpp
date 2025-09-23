#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace AEngine {

Camera::Camera()
    : m_position(0.0f, 0.0f, 3.0f)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
{
    updateViewMatrix();
    // ใช้ค่าเริ่มต้นที่กำหนดใน .h มาสร้าง projection matrix ตั้งแต่แรก
    setPerspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::setPerspective(float fov, float aspect, float near, float far) {
    // เก็บค่าทั้งหมดไว้ใน member variables
    m_fov = fov;
    m_aspectRatio = aspect;
    m_nearPlane = near;
    m_farPlane = far;    
    m_projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::setAspectRatio(float aspect) {
    setPerspective(m_fov, aspect, m_nearPlane, m_farPlane);
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