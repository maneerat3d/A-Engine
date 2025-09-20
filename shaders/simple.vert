#version 450 core

// รับข้อมูลตำแหน่งของ vertex เข้ามาทาง attribute ที่ 0
layout (location = 0) in vec3 aPos;

void main()
{
    // กำหนดตำแหน่งสุดท้ายของ vertex (ไม่ต้องทำอะไร ส่งไปตรงๆ)
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}