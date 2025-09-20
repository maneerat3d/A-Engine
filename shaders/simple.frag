#version 450 core

// กำหนดตัวแปรสำหรับสีที่จะส่งออกไป
out vec4 FragColor;
// รับ TexCoord ที่ถูก Interpolate แล้วมาจาก Vertex Shader
in vec2 TexCoord;

// รับ Texture Sampler เข้ามาเป็น Uniform
uniform sampler2D ourTexture;

void main()
{
    // ดึงค่าสีจาก Texture ณ พิกัด TexCoord ปัจจุบัน
    FragColor = texture(ourTexture, TexCoord);
}