# สร้าง library หลักของ engine A-Engine/src/engine/CMakeLists.txt
add_library(aengine_engine
    engine.cpp
    engine.h
    game_system.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/platform_subsystem.cpp
)

# กำหนด include directories สำหรับ aengine_engine
target_include_directories(aengine_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_SOURCE_DIR}/external/SDL2/include
        ${CMAKE_SOURCE_DIR}/external/glad/include
)

# --- ส่วนของ game_plugin ---

# 1. สร้าง library สำหรับ game_plugin โดยเฉพาะ
add_library(game_plugin game_plugin.cpp)

# 2. บอกให้ game_plugin รู้จักกับ aengine_core เพื่อหา iplugin.h
target_link_libraries(game_plugin PUBLIC aengine_core)

# --- ส่วนของการ Link ---

# 3. ให้ aengine_engine เรียกใช้ aengine_core, aengine_renderer และ game_plugin ที่เราสร้างขึ้นใหม่
target_link_libraries(aengine_engine PUBLIC
    aengine_core
    aengine_renderer
    game_plugin # เพิ่ม game_plugin เข้ามา
    SDL2::SDL2
    OpenGL::GL
)