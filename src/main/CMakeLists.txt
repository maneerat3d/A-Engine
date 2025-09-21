# สร้าง Executable ชื่อ aengine_main
add_executable(aengine_main main.cpp)

# Executable ของเราต้องพึ่งพา Engine (ซึ่งก็จะดึง Core และ Renderer มาเอง)
target_link_libraries(aengine_main 
    PRIVATE 
        aengine_engine
)

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

# === ขั้นตอน Post-Build (ยังคงเหมือนเดิม) ===
add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL2/lib/x64/SDL2.dll"
        "$<TARGET_FILE_DIR:aengine_main>"
)
add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:aengine_main>/shaders"
)
add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/textures"
        "$<TARGET_FILE_DIR:aengine_main>/textures"
)

add_custom_command(
    TARGET aengine_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/models"
        "$<TARGET_FILE_DIR:aengine_main>/models"
    COMMENT "Copying models to output directory..."
)