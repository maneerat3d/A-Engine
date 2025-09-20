#include "engine.h"

// include ทุกอย่างที่จำเป็นที่นี่
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


// === โค้ดส่วน Helper (จะย้ายออกไปทีหลัง) ===
namespace { // Anonymous namespace for helper functions

void log_sdl_error(const char* msg) {
    std::cerr << msg << ": " << SDL_GetError() << std::endl;
}

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

unsigned int create_shader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

unsigned int create_shader_program(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return program;
}

} // end anonymous namespace


namespace AEngine {

Engine::Engine() 
    : m_window(nullptr)
    , m_is_running(false)
    , m_gl_context(nullptr)
    , m_shader_program(0)
    , m_vao(0)
    , m_vbo(0)
    , m_texture(0)
    , m_model_loc(-1)
    , m_view_loc(-1)
    , m_projection_loc(-1)
{
}

Engine::~Engine() {
    // Destructor ควรจะว่างเปล่า, การจัดการทรัพยากรควรอยู่ใน shutdown()
}

void Engine::run() {
    init();
    gameLoop();
    shutdown();
}

void Engine::init() {
    std::cout << "A-Engine is initializing..." << std::endl;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_sdl_error("SDL could not initialize!");
        return;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow("A-Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!m_window) {
        log_sdl_error("Window could not be created!");
        return;
    }
    
    m_gl_context = SDL_GL_CreateContext(m_window);
    if (!m_gl_context) {
        log_sdl_error("OpenGL context could not be created!");
        return;
    }
    
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    // === สร้าง Shaders ===
    std::string vertSource = read_shader_file("shaders/simple.vert");
    std::string fragSource = read_shader_file("shaders/simple.frag");
    unsigned int vertexShader = create_shader(vertSource.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = create_shader(fragSource.c_str(), GL_FRAGMENT_SHADER);
    m_shader_program = create_shader_program(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // === ค้นหาตำแหน่งของ Uniforms ===
    glUseProgram(m_shader_program);
    m_model_loc = glGetUniformLocation(m_shader_program, "model");
    m_view_loc = glGetUniformLocation(m_shader_program, "view");
    m_projection_loc = glGetUniformLocation(m_shader_program, "projection");
    glUniform1i(glGetUniformLocation(m_shader_program, "ourTexture"), 0);

    // === เตรียมข้อมูล Vertex ของลูกบาศก์ ===
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
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // === สร้างและโหลด Texture ===
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    m_is_running = true;
}

void Engine::gameLoop() {
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    while (m_is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_is_running = false;
            }
        }

        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_shader_program);

        // --- ส่ง Uniforms ของแสงและมุมมอง ---
        glUniform3f(glGetUniformLocation(m_shader_program, "lightPos"), 1.2f, 1.0f, 2.0f);
        glUniform3fv(glGetUniformLocation(m_shader_program, "viewPos"), 1, &cameraPos[0]);
        glUniform3f(glGetUniformLocation(m_shader_program, "lightColor"), 1.0f, 1.0f, 1.0f);

        // --- สร้างและส่ง Matrices (MVP) ---
        glm::mat4 model = glm::mat4(1.0f);
        float time = (float)SDL_GetTicks() / 1000.0f;
        model = glm::rotate(model, time, glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, &model[0][0]);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, &view[0][0]);

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f
        );
        glUniformMatrix4fv(m_projection_loc, 1, GL_FALSE, &projection[0][0]);
        
        // *** Bind Texture ก่อนวาด ***
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        SDL_GL_SwapWindow(m_window);
    }
}

void Engine::shutdown() {
    std::cout << "A-Engine is shutting down." << std::endl;

    // คืนทรัพยากรทั้งหมด
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteProgram(m_shader_program);
    glDeleteTextures(1, &m_texture);

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

} // namespace AEngine