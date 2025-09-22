#include "renderer.h"
#include "core/world/world.h"
#include "mesh.h"
#include "texture.h"
// include ทุกอย่างที่จำเป็นสำหรับการเรนเดอร์
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// === โค้ดส่วน Helper ที่ย้ายมาจาก engine.cpp ===
namespace { // Anonymous namespace for helper functions

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

Renderer::Renderer()
    : m_shader_program(0)
    , m_model_loc(-1)
    , m_view_loc(-1)
    , m_projection_loc(-1)
    , m_window(nullptr)
{
}

Renderer::~Renderer() {
}

void Renderer::init(SDL_Window* window) {
    m_window = window;

    glEnable(GL_DEPTH_TEST);

    // === สร้าง Shaders === (เหมือนเดิม)
    std::string vertSource = read_shader_file("shaders/simple.vert");
    std::string fragSource = read_shader_file("shaders/simple.frag");
    unsigned int vertexShader = create_shader(vertSource.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = create_shader(fragSource.c_str(), GL_FRAGMENT_SHADER);
    m_shader_program = create_shader_program(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // === ค้นหาตำแหน่งของ Uniforms === (เหมือนเดิม)
    glUseProgram(m_shader_program);
    m_model_loc = glGetUniformLocation(m_shader_program, "model");
    m_view_loc = glGetUniformLocation(m_shader_program, "view");
    m_projection_loc = glGetUniformLocation(m_shader_program, "projection");
    glUniform1i(glGetUniformLocation(m_shader_program, "ourTexture"), 0);
}

void Renderer::render(World& world) {

     glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // ใช้ Camera จาก Scene
    Camera& camera = world.getCamera();
    const glm::mat4& view = camera.getViewMatrix();
    const glm::mat4& projection = camera.getProjectionMatrix();
    const glm::vec3& cameraPos = camera.getPosition();

    glUseProgram(m_shader_program);

    // --- ส่ง Uniforms ของแสงและมุมมอง ---
    glUniform3f(glGetUniformLocation(m_shader_program, "lightPos"), 1.2f, 1.0f, 2.0f);
    glUniform3fv(glGetUniformLocation(m_shader_program, "viewPos"), 1, &cameraPos[0]);
    glUniform3f(glGetUniformLocation(m_shader_program, "lightColor"), 1.0f, 1.0f, 1.0f);

    // --- ส่ง View และ Projection Matrices (ครั้งเดียวต่อเฟรม) ---
    glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(m_projection_loc, 1, GL_FALSE, &projection[0][0]);

    
    // --- วนลูปวาด GameObject ทุกชิ้นใน Scene ---
    // นี่คือ Logic ของ RenderSystem
    for (Entity i = 1; i < world.getEntityCount(); ++i) {
        auto& renderable = world.getComponent<ECS::RenderableComponent>(i);
        if (renderable.mesh) {
            auto texture = renderable.mesh->getTexture();
            if (texture) {
                texture->bind();
            }
            
            renderable.mesh->bind();

            auto& transform = world.getComponent<ECS::TransformComponent>(i);
            glm::mat4 model = transform.getMatrix();            
            glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, &model[0][0]);
            glDrawElements(GL_TRIANGLES, renderable.mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
    }
}

void Renderer::shutdown() {
    // คืนทรัพยากรทั้งหมด
    glDeleteProgram(m_shader_program);
}

} // namespace AEngine