# กำหนดเวอร์ชันขั้นต่ำของ CMake
cmake_minimum_required(VERSION 3.10)

# กำหนดชื่อโปรเจกต์
project(imgui)

# สร้างไลบรารีแบบ static
add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    backends/imgui_impl_sdl2.cpp
    backends/imgui_impl_opengl3.cpp
)

# บอกให้ ImGui หา header ของตัวเอง และ header ของ SDL2 เจอ
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/SDL2/include
)

# สำหรับ Static Linking บน Windows, เราต้องการให้ IMGUI_API เป็นค่าว่าง
target_compile_definitions(imgui PUBLIC IMGUI_API=)