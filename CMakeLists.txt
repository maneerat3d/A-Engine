# ระบุเวอร์ชั่นขั้นต่ำของ CMake ที่ต้องการ
cmake_minimum_required(VERSION 3.10)

# กำหนดชื่อโปรเจกต์
project(A-Engine)

# บอก CMake ว่าเราจะใช้ C++20 และให้เปิดใช้งาน Extension ของ Compiler ด้วย
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# สร้าง Executable (โปรแกรม .exe) ชื่อ A-Engine จากไฟล์ main.cpp
add_executable(A-Engine src/main.cpp)