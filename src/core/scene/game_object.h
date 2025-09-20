#pragma once

#include "transform.h"
#include <string>

namespace AEngine {

struct GameObject {
    Transform transform;
    std::string name;

    // TODO: Phase 3 จะเปลี่ยนเป็นระบบ Component
    // ข้อมูลสำหรับ Renderer จะ hardcode ไปก่อน
    int modelId = 0;
    int textureId = 0;
};

} // namespace AEngine