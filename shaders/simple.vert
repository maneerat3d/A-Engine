#version 450 core

// รับข้อมูลตำแหน่งของ vertex เข้ามาทาง attribute ที่ 0
layout (location = 0) in vec3 aPos;

// รับ Model Matrix เข้ามาเป็น Uniform
uniform mat4 model;

void main()
{
    // กำหนดตำแหน่งสุดท้ายของ vertex (ไม่ต้องทำอะไร ส่งไปตรงๆ)
    // คูณตำแหน่งของ Vertex ด้วย Model Matrix เพื่อแปลงตำแหน่ง
    gl_Position = model * vec4(aPos, 1.0);
}