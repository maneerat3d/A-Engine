#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// ดึง Header ของ SDL เข้ามาใช้งาน
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad/glad.h>

// === Include STB Image Loader ===
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// ดึง Header ของ GLM เข้ามาใช้งาน
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ฟังก์ชันสำหรับจัดการข้อผิดพลาดของ SDL
void log_sdl_error(const char* msg) {
    std::cerr << msg << ": " << SDL_GetError() << std::endl;
}
 
// ฟังก์ชันสำหรับอ่านไฟล์ Shader ทั้งหมดมาเป็น string
std::string read_shader_file(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
        return "";
    }
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    return stream.str();
}

// ฟังก์ชันสำหรับสร้างและ Compile Shader
unsigned int create_shader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // ตรวจสอบ Error
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// ฟังก์ชันสำหรับสร้าง Shader Program จาก Vertex และ Fragment shader
unsigned int create_shader_program(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // ตรวจสอบ Error
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return program;
}

int main() {
    std::cout << "A-Engine is starting..." << std::endl;

    // 1. เริ่มต้นการทำงานของระบบ SDL (ในที่นี้คือระบบ Video)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_sdl_error("Failed to initialize SDL");
        return -1;
    }

    // === ตั้งค่าเวอร์ชั่นของ OpenGL ที่เราต้องการ ===
    // เราจะใช้ OpenGL 3.3 ซึ่งเป็นเวอร์ชั่นที่ทันสมัยและรองรับได้กว้าง
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    // ใช้ Core Profile คือไม่ใช้ฟังก์ชันเก่าๆ ของ OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // เปิดใช้งาน Double Buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    
    // 2. สร้างหน้าต่าง
    SDL_Window* window = SDL_CreateWindow(
        "A-Engine",                 // Title ของหน้าต่าง
        SDL_WINDOWPOS_CENTERED,     // ตำแหน่ง X (กึ่งกลาง)
        SDL_WINDOWPOS_CENTERED,     // ตำแหน่ง Y (กึ่งกลาง)
        1280,                       // ความกว้าง
        720,                        // ความสูง
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // Flag เพิ่มเติม
    );

    if (!window) {
        log_sdl_error("Failed to create window");
        SDL_Quit();
        return -1;
    }

    // 3. สร้าง OpenGL Context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        log_sdl_error("Failed to create OpenGL context");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 4. โหลดฟังก์ชันทั้งหมดของ OpenGL ด้วย GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // === สร้าง Shaders ===
    std::string vertSource = read_shader_file("shaders/simple.vert");
    std::string fragSource = read_shader_file("shaders/simple.frag");
    unsigned int vertexShader = create_shader(vertSource.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = create_shader(fragSource.c_str(), GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = create_shader_program(vertexShader, fragmentShader);

    // === เปิดใช้งาน Depth Test ===
    glEnable(GL_DEPTH_TEST);

    // --- ค้นหาตำแหน่งของ Uniforms ทั้งหมดหลังสร้าง Program (ทำครั้งเดียว) ---
    // การทำแบบนี้จะมีประสิทธิภาพกว่าการค้นหาในลูปทุกเฟรม
    glUseProgram(shaderProgram); // ต้องสั่ง use program ก่อน ถึงจะหา location เจอ
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projectionLoc = glGetUniformLocation(shaderProgram, "projection"); 
    // บอก OpenGL ว่า uniform `ourTexture` ของเราจะใช้ Texture Unit 0
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
   
    // ลบ Shader object ทิ้งได้เลยเมื่อ Link เสร็จแล้ว
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
     // === เตรียมข้อมูล Vertex ของลูกบาศก์ (เพิ่ม Texture Coords) ===
     //      ตำแหน่ง X, Y, Z,   TexCoord U, V
     float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
     };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 1. Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 2. Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 3. Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // === สร้างและโหลด Texture ===
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // ตั้งค่า Texture wrapping/filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // โหลดไฟล์รูปภาพด้วย stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // พลิกรูปให้ตรงกับระบบพิกัดของ OpenGL
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // ส่งข้อมูลรูปภาพไปให้ Texture Object ที่เรา Bind ไว้อยู่
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // สร้าง Mipmap อัตโนมัติ
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    // คืนหน่วยความจำหลังโหลดเสร็จ
    stbi_image_free(data);


    bool is_running = true;
    SDL_Event event;

    // นี่คือ Game Loop ของเรา!
    while (is_running) {
        
        // 1. จัดการ Input (Event Pumping)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }
        // สร้างตำแหน่งกล้องขึ้นมา
        glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
        
        // - ล้างหน้าจอด้วยสีน้ำเงิน
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        
        // - *** อย่าลืมล้าง Depth Buffer ด้วย! ***
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // - สั่งวาดสามเหลี่ยม
        glUseProgram(shaderProgram);      // บอก GPU ว่าจะใช้ Shader Program ไหน        

        // --- ส่ง Uniforms ของแสงและมุมมอง ---
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 1.2f, 1.0f, 2.0f);
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &cameraPos[0]);
        glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    

        // --- สร้างและส่ง Matrices (MVP) ---
        // 1. Model Matrix
        glm::mat4 model = glm::mat4(1.0f);
        float time = (float)SDL_GetTicks() / 1000.0f;
        model = glm::rotate(model, time, glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        // 2. View Matrix
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);


        // 3. Projection Matrix: สร้างเลนส์แบบ Perspective
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),      // Field of View
            1280.0f / 720.0f,         // Aspect Ratio
            0.1f,                     // Near clipping plane
            100.0f                    // Far clipping plane
        );

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
 

        glBindVertexArray(VAO);           

        // *** Bind Texture ก่อนวาด ***
        glActiveTexture(GL_TEXTURE0); // เปิดใช้งาน Texture Unit 0
        glBindTexture(GL_TEXTURE_2D, texture);

        // วาดลูกบาศก์ (36 vertices)
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // - สลับ Buffer เพื่อแสดงผล
        
        SDL_GL_SwapWindow(window);
    }


    // คืนทรัพยากรทั้งหมด
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);    
    glDeleteProgram(shaderProgram);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "A-Engine has shut down." << std::endl;
    return 0;
}