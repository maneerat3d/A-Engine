add_library(aengine_renderer
    dummy.cpp
)

target_include_directories(aengine_renderer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Renderer ต้องพึ่งพา Core
target_link_libraries(aengine_renderer PRIVATE aengine_core)