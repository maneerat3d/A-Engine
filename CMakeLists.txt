# ระบุเวอร์ชั่นขั้นต่ำของ CMake ที่ต้องการ
cmake_minimum_required(VERSION 3.10)

# กำหนดชื่อโปรเจกต์
project(A-Engine)

# บอก CMake ว่าเราจะใช้ C++20 และให้เปิดใช้งาน Extension ของ Compiler ด้วย
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# --- จัดการ Dependencies ---
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/external)


find_package(SDL2 REQUIRED)

# เพิ่มไฟล์ source ของ GLAD เข้ามาในโปรเจกต์
include_directories(external/glad/include)

# --- สร้าง Target ---
add_executable(A-Engine
    src/main.cpp
    external/glad/src/glad.c
)


# เพิ่ม Path ของ header ไฟล์ SDL2 เข้าไปในโปรเจกต์
target_include_directories(A-Engine PRIVATE ${SDL2_INCLUDE_DIRS})
# ทำการ Link ไฟล์ Library ของ SDL2 เข้ากับโปรแกรมของเรา
target_link_libraries(A-Engine PRIVATE ${SDL2_LIBRARIES} opengl32)

add_custom_command(
    TARGET A-Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL2/lib/x64/SDL2.dll"
        "$<TARGET_FILE_DIR:A-Engine>"
    COMMENT "Copying SDL2.dll to output directory"
)

add_custom_command(
    TARGET A-Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:A-Engine>/shaders"
)