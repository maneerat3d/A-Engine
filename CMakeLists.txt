# ระบุเวอร์ชั่นและชื่อโปรเจกต์ A-Engine/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(A-Engine)

# --- เพิ่มส่วนนี้เข้ามา ---
# 1. สร้าง library ชื่อ 'glad' จากไฟล์ glad.c
add_library(glad external/glad/src/glad.c)
# 2. บอกว่าใครก็ตามที่ใช้ glad ต้อง include จากโฟลเดอร์นี้ด้วย
target_include_directories(glad PUBLIC external/glad/include)
# --- จบส่วนที่เพิ่ม ---

# ตั้งค่า C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# บอก CMake ให้หาไฟล์สำหรับตั้งค่า Package ในโฟลเดอร์ external
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/external)

# ค้นหา Dependencies ที่เราจะใช้
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)


# --- ImGui ---
add_subdirectory(external/imgui)

# เพิ่ม Subdirectories ของแต่ละ Module
# CMake จะเข้าไปอ่าน CMakeLists.txt ในโฟลเดอร์เหล่านี้
add_subdirectory(external/cgltf)
add_subdirectory(src/core)
add_subdirectory(src/renderer)
add_subdirectory(src/engine)
add_subdirectory(src/editor)
add_subdirectory(src/plugins) 
add_subdirectory(src/main)