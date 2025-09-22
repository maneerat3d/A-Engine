# สร้าง Library ชื่อ "renderer" A-Engine/src/renderer/CMakeLists.txt
add_library(aengine_renderer
    mesh.cpp
    render_system.cpp
    renderer.cpp
    texture.cpp
)

target_link_libraries(aengine_renderer PRIVATE 
    aengine_core # ใช้ชื่อที่สอดคล้องกัน
    glad
    opengl32
)


# สร้าง Library สำหรับ Renderer Plugin (มีแค่ครั้งเดียว)
add_library(renderer_plugin renderer_plugin.cpp)

target_link_libraries(renderer_plugin PRIVATE 
    aengine_core 
    aengine_renderer
)



# บอกว่า renderer ต้อง include header จากที่ไหนบ้าง
target_include_directories(aengine_renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/scene
    ${CMAKE_SOURCE_DIR}/src/core/resource
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/stb
)


target_link_libraries(aengine_renderer 
    PRIVATE
        aengine_core
        glad
        opengl32
        SDL2::SDL2
        cgltf
)

