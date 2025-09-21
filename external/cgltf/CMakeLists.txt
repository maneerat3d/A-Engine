# สร้าง library สำหรับ cgltf
add_library(cgltf
    cgltf_impl.cpp
    cgltf.h
)

target_include_directories(cgltf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)