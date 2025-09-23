# กำหนดเวอร์ชันขั้นต่ำของ CMake
cmake_minimum_required(VERSION 3.10)

# กำหนดชื่อโปรเจกต์
project(freetype)

# สร้างไลบรารีแบบ IMPORTED STATIC ชื่อว่า "freetype"
# "IMPORTED" หมายความว่าไลบรารีนี้มีไฟล์ .lib/.a อยู่แล้ว ไม่ต้องคอมไพล์ใหม่
add_library(freetype STATIC IMPORTED)

# ตั้งค่า Properties ที่สำคัญให้กับไลบรารี "freetype"
set_target_properties(freetype PROPERTIES
    # บอก CMake ว่าไฟล์ไลบรารี (.lib) จริงๆ นั้นอยู่ที่ไหน
    # เราจะใช้กับ Windows เท่านั้นตามไฟล์ที่มีในโปรเจกต์
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/freetype.lib"

    # บอกว่าโปรเจกต์อื่นที่นำไลบรารีนี้ไปใช้ (link) จะต้อง include path ไหน
    # เพื่อให้สามารถ #include <freetype/freetype.h> ได้
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)