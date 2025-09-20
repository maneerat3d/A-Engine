add_library(aengine_engine
    engine.cpp
)

target_include_directories(aengine_engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Engine ต้องพึ่งพา Core และ Renderer
# และต้องใช้ SDL2, GLAD, GLM, OpenGL
target_link_libraries(aengine_engine 
    PRIVATE
        aengine_core
        aengine_renderer
        SDL2::SDL2
        OpenGL::GL
)

target_include_directories(aengine_engine 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external
)