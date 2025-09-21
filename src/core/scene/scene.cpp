#include "scene.h"

namespace AEngine {

Scene::Scene() {
}

Scene::~Scene() {
}

Entity Scene::createEntity() {
    // ยังไม่มีระบบ re-use ID, แค่เพิ่มไปเรื่อยๆ ก่อน
    // และต้องแน่ใจว่าไม่เกินขนาด Array ที่จองไว้ (1024)
    if (m_entityCounter >= 1024) {
        // In a real engine, we'd assert or handle this gracefully
        return INVALID_ENTITY;
    }
    return m_entityCounter++;
}

} // namespace AEngine