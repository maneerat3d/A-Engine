

add_library(TestLogPlugin SHARED
    TestLogPlugin.cpp
)


 # ระบุว่าโปรเจกต์นี้ต้อง include อะไรบ้าง
 # เราต้องการ IPlugin.h ซึ่งอยู่ใน src/core/plugins
 # ${CMAKE_SOURCE_DIR} คือ root ของโปรเจกต์ (โฟลเดอร์ที่มี A-Engine/CMakeLists.txt)
 target_include_directories(TestLogPlugin PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
)

# Link library ที่จำเป็นสำหรับ plugin นี้
target_link_libraries(gltf_importer_plugin PRIVATE
    aengine_core
    aengine_renderer
    cgltf
)

# กำหนด preprocessor macro "MYLIB_EXPORTS" เมื่อทำการคอมไพล์ไลบรารีนี้
target_compile_definitions(TestLogPlugin PRIVATE MYLIB_EXPORTS)

 # (สำคัญ) ตั้งค่าตำแหน่งไฟล์ Output
 # เพื่อให้ทั้ง .dll และ .exe ไปอยู่ในโฟลเดอร์เดียวกัน (build/Debug หรือ build/Release)
 # ทำให้ .exe สามารถหา .dll เจอตอนรัน
 set_target_properties(TestLogPlugin PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
 )