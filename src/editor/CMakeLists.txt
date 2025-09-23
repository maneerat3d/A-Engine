# สร้าง Library สำหรับ Editor A-Engine/src/editor/CMakeLists.txt
add_library(aengine_editor
    editor.cpp
    ui/editor_ui.cpp
    scene_view.cpp
)


# บอกว่า editor ต้อง include header จากที่ไหนบ้าง
target_include_directories(aengine_editor PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/portable-file-dialogs/
)

# Link Editor เข้ากับ Library ที่ต้องใช้
target_link_libraries(aengine_editor PUBLIC
    aengine_engine
    # glad และ opengl32 จำเป็นสำหรับ Framebuffer
    glad
    opengl32
    imgui 
)