#include "engine.h"

// include ทุกอย่างที่จำเป็นที่นี่
#include <iostream>
#include <SDL.h>
#include <glad/glad.h>

namespace AEngine {

Engine::Engine() 
    : m_window(nullptr)
    , m_is_running(false)
{
}

Engine::~Engine() {
    // ตรวจสอบว่า shutdown ถูกเรียกแล้ว
}

void Engine::run() {
    init();
    gameLoop();
    shutdown();
}

void Engine::init() {
    std::cout << "A-Engine is initializing..." << std::endl;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow("A-Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    SDL_GL_CreateContext(m_window);
    
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    glEnable(GL_DEPTH_TEST);

    m_is_running = true;
}

void Engine::gameLoop() {
    while (m_is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_is_running = false;
            }
        }

        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // โค้ดสำหรับวาดลูกบาศก์ของเรายังคงอยู่ที่นี่ (เราจะย้ายมันไป Renderer ทีหลัง)
        // ... (นำโค้ดส่วนสร้าง shader, vertex data, uniforms, และ draw calls จาก main เก่ามาไว้ที่นี่) ...
        // หมายเหตุ: เพื่อให้ compile ผ่านก่อน เราอาจจะต้อง comment ส่วนวาดรูปไปก่อน

        SDL_GL_SwapWindow(m_window);
    }
}

void Engine::shutdown() {
    std::cout << "A-Engine is shutting down." << std::endl;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

} // namespace AEngine