add_library(aengine_engine
    engine.cpp
    engine.h    
)

target_include_directories(aengine_engine 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/core      
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_SOURCE_DIR}/external/SDL2/include  
        ${CMAKE_SOURCE_DIR}/external/glad/include
)

# Engine ต้องพึ่งพา Core และ Renderer
# และต้องใช้ SDL2, GLAD, GLM, OpenGL
target_link_libraries(aengine_engine PUBLIC
    aengine_renderer
    SDL2::SDL2
    OpenGL::GL
)

