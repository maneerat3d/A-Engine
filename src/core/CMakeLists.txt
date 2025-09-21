# สร้าง Library สำหรับ Core Module
add_library(aengine_core
    scene/camera.cpp
    scene/scene.cpp
    resource/resource_manager.cpp 
    # Add new plugin/system interfaces here
    # Note: These are headers, but since 'core' is a full library now, it's fine.
)

# บอกว่า core ต้อง include header จากที่ไหนบ้าง
target_include_directories(aengine_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    # เราต้องใช้ glm ใน transform.h ดังนั้นจึงต้องเพิ่มที่นี่
    ${CMAKE_SOURCE_DIR}/external/
)