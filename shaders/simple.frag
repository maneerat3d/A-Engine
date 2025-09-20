#version 450 core

// กำหนดตัวแปรสำหรับสีที่จะส่งออกไป
out vec4 FragColor;

void main()
{
    // กำหนดให้ทุกพิกเซลเป็นสีขาวทึบ (RGBA)
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}