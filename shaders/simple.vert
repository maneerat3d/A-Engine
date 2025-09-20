#version 450 core

// รับข้อมูลตำแหน่งของ vertex เข้ามาทาง attribute ที่ 0
layout (location = 0) in vec3 aPos;
// รับข้อมูล Normal Vector เข้ามาทาง attribute ที่ 2
layout (location = 1) in vec3 aNormal;
 // รับข้อมูล Texture Coordinate เข้ามาทาง attribute ที่ 1
layout (location = 2) in vec2 aTexCoord; 


// รับ Matrix ทั้ง 3 ตัวเข้ามาเป็น Uniform
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ส่งข้อมูลต่อไปให้ Fragment Shader
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos; // ตำแหน่งของ fragment ใน world space

void main()
{
    // คำนวณตำแหน่งสุดท้ายด้วย MVP Pipeline
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // ส่ง TexCoord ออกไปตรงๆ
    TexCoord = aTexCoord;
    // คำนวณตำแหน่งใน World Space
    FragPos = vec3(model * vec4(aPos, 1.0));
    // คำนวณ Normal ใน World Space ให้ถูกต้อง
    Normal = mat3(transpose(inverse(model))) * aNormal;
}