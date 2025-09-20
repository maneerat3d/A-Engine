#version 450 core



// รับ TexCoord ที่ถูก Interpolate แล้วมาจาก Vertex Shader
in vec2 TexCoord;

// รับ Normal และตำแหน่งใน World Space
in vec3 Normal;
in vec3 FragPos;

// รับ Texture Sampler เข้ามาเป็น Uniform
uniform sampler2D ourTexture;

// รับค่าเกี่ยวกับแสงและมุมมอง
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

// กำหนดตัวแปรสำหรับสีที่จะส่งออกไป
out vec4 FragColor;

void main()
{
    // --- คำนวณแสงแบบ Phong ---

    // 1. Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 3. Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // --- รวมผลลัพธ์ ---
    vec3 result = (ambient + diffuse + specular) * texture(ourTexture, TexCoord).rgb;
    //vec3 result = texture(ourTexture, TexCoord).rgb;
    //vec3 result = (ambient + diffuse + specular);
    //vec3 result = normalize(Normal);
    FragColor = vec4(result, 1.0);
}