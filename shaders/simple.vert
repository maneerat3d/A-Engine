#version 450 core

// รับข้อมูลตำแหน่งของ vertex เข้ามาทาง attribute ที่ 0
layout (location = 0) in vec3 aPos;
// รับข้อมูล Texture Coordinate เข้ามาทาง attribute ที่ 1
layout (location = 1) in vec2 aTexCoord;

// รับ Matrix ทั้ง 3 ตัวเข้ามาเป็น Uniform
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ส่ง Texture Coordinate ต่อไปให้ Fragment Shader
out vec2 TexCoord;

void main()
{
    // คำนวณตำแหน่งสุดท้ายด้วย MVP Pipeline
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // ส่ง TexCoord ออกไปตรงๆ
    TexCoord = aTexCoord;
}