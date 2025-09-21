# ระบุเวอร์ชั่นและชื่อโปรเจกต์
cmake_minimum_required(VERSION 3.16)
project(A-Engine)

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

# เพิ่ม Subdirectories ของแต่ละ Module
# CMake จะเข้าไปอ่าน CMakeLists.txt ในโฟลเดอร์เหล่านี้
add_subdirectory(external/cgltf)
add_subdirectory(src/core)
add_subdirectory(src/renderer)
add_subdirectory(src/engine)
add_subdirectory(src/main)