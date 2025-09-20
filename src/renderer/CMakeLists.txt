add_library(aengine_renderer
    mesh.cpp
    texture.cpp
    renderer.cpp
)

target_include_directories(aengine_renderer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# บอกว่า renderer ต้อง include header จาก core module ได้
target_include_directories(aengine_renderer PRIVATE ${CMAKE_SOURCE_DIR}/src/core)


# บอกว่า renderer ต้อง include header จากที่ไหนบ้าง
target_include_directories(aengine_renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/scene
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/stb
)


target_link_libraries(aengine_renderer 
    PRIVATE
        aengine_core
        SDL2::SDL2
        OpenGL::GL
)

