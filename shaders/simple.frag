#version 450 core

// กำหนดตัวแปรสำหรับสีที่จะส่งออกไป
out vec4 FragColor;
// รับค่าสีเข้ามาทาง Uniform
uniform vec4 ourColor;
void main()
{
    // ใช้สีที่ได้รับมาจาก Uniform
    FragColor = ourColor;
}