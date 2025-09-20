#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// ดึง Header ของ SDL เข้ามาใช้งาน
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad/glad.h>

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

    // ลบ Shader object ทิ้งได้เลยเมื่อ Link เสร็จแล้ว
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // === เตรียมข้อมูล Vertex ของสามเหลี่ยม ===
    //      ตำแหน่ง X, Y, Z
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };
    // === สร้าง Index Buffer ===
    // นิยามสามเหลี่ยม 12 รูป (2 รูปต่อ 1 ด้าน) โดยใช้ index อ้างอิงถึง vertices ข้างบน
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 4, 7, 7, 3, 0,
        1, 5, 6, 6, 2, 1,
        3, 7, 6, 6, 2, 3,
        0, 4, 5, 5, 1, 0
     };

    // === สร้าง Buffer บน GPU ===
    unsigned int VBO, VAO, EBO;
    // 1. สร้าง Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    // 2. สร้าง Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);
    // 3. สร้าง Element Buffer Object (EBO/IBO)
    glGenBuffers(1, &EBO);
    // 4. Bind VAO ก่อน เพื่อให้การตั้งค่าทั้งหมดถูกบันทึกไว้ใน VAO นี้
    glBindVertexArray(VAO);

    // 5. Bind VBO และส่งข้อมูล vertices ของเราเข้าไป
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 6. Bind EBO และส่งข้อมูล indices ของเราเข้าไป
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 7. บอก OpenGL ว่าจะอ่านข้อมูลใน VBO อย่างไร (Vertex Attribute Pointer)
    // - location = 0 ตรงกับ `layout (location = 0)` ใน Vertex shader
    // - 3 คือขนาดของข้อมูล (vec3)
    // - GL_FLOAT คือชนิดข้อมูล
    // - GL_FALSE คือไม่ต้อง normalize
    // - 3 * sizeof(float) คือ Stride หรือขนาดของ 1 vertex
    // - (void*)0 คือ offset เริ่มต้น
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // เปิดใช้งาน attribute ที่ 0

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
        
        // - ล้างหน้าจอด้วยสีน้ำเงิน
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        
        // - *** อย่าลืมล้าง Depth Buffer ด้วย! ***
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // - สั่งวาดสามเหลี่ยม
        glUseProgram(shaderProgram);      // บอก GPU ว่าจะใช้ Shader Program ไหน        

        // --- สร้างและส่ง Transformation Matrices (MVP) ---        

        // 1. Model Matrix: หมุนลูกบาศก์รอบแกนที่น่าสนใจขึ้น
        glm::mat4 model = glm::mat4(1.0f);
        float time = (float)SDL_GetTicks() / 1000.0f;
        model = glm::rotate(model, time, glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        // 2. View Matrix: ตั้งกล้องถอยหลังจากจุดกำเนิด 3 หน่วย และมองไปที่จุดกำเนิด
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0, 0.0, 3.0), // ตำแหน่งกล้อง (Eye)
            glm::vec3(0.0, 0.0, 0.0), // จุดที่มอง (Center)
            glm::vec3(0.0, 1.0, 0.0)  // ทิศบน (Up)
        );
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
        // *** เปลี่ยนมาใช้ glDrawElements เพื่อวาดโดยอ้างอิงจาก Index Buffer ***
        // - วาดสามเหลี่ยม, จำนวน 36 indices, ชนิดของ index คือ unsigned int
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // - สลับ Buffer เพื่อแสดงผล
        
        SDL_GL_SwapWindow(window);
    }


    // คืนทรัพยากรทั้งหมด
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);    
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "A-Engine has shut down." << std::endl;
    return 0;
}