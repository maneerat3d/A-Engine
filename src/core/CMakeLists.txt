# สร้าง Library ชื่อ aengine_core
add_library(aengine_core
    dummy.cpp
)

# ทำให้ Library อื่นๆ สามารถเห็น Header ของ core ได้
target_include_directories(aengine_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)