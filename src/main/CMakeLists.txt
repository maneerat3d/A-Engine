# สร้างไฟล์ Executable ของโปรแกรม
add_executable(aengine_main main.cpp)

# Link Executable เข้ากับ Library ทั้งหมดที่ต้องใช้
# การเพิ่ม plugin library ทั้งหมดเข้ามาตรงนี้ คือการแก้ปัญหา Linker Error
target_link_libraries(aengine_main 
    PUBLIC
        aengine_core
    PRIVATE
        aengine_editor    
        aengine_engine
        renderer_plugin
        game_plugin
        gltf_importer_plugin
        SDL2::SDL2
        glad
        opengl32
        imgui
)


# --- การตั้งค่าสำหรับ GLAD (เพื่อให้ main.cpp ใช้งานได้) ---

# เพิ่มไฟล์ source ของ GLAD เข้ามาในโปรเจกต์หลัก
target_sources(aengine_main 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

# เพิ่ม Include Path สำหรับ glad.c เพื่อให้หา glad/glad.h เจอ
target_include_directories(aengine_main
    PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_include_directories(aengine_main PRIVATE 
    "${CMAKE_SOURCE_DIR}/src"
)

# --- ขั้นตอน Post-Build สำหรับคัดลอกไฟล์ที่จำเป็น ---

# คัดลอก SDL2.dll
add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL2/lib/x64/SDL2.dll"
        "$<TARGET_FILE_DIR:aengine_main>"
    COMMENT "Copying SDL2.dll..."
)

# คัดลอกโฟลเดอร์ assets ต่างๆ
add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:aengine_main>/shaders"
    COMMENT "Copying shaders..."
)

add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/textures"
        "$<TARGET_FILE_DIR:aengine_main>/textures"
    COMMENT "Copying textures..."
)

add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/models"
        "$<TARGET_FILE_DIR:aengine_main>/models"
    COMMENT "Copying models..."
)