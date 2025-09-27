# src/plugins/gltf_importer/CMakeLists.txt

add_library(gltf_importer_plugin
    gltf_importer_plugin.cpp
)

# ทำให้ภายนอกสามารถ include header จาก plugin นี้ได้
target_include_directories(gltf_importer_plugin PUBLIC
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