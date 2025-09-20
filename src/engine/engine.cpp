#include "engine.h"
#include "renderer.h"
// include ทุกอย่างที่จำเป็นที่นี่
#include <iostream>

#include <SDL.h>
#include <glad/glad.h>

// ย้าย Helper functions ไปที่ renderer.cpp หมดแล้ว เหลือแค่ log_sdl_error
namespace {
void log_sdl_error(const char* msg) {
    std::cerr << msg << ": " << SDL_GetError() << std::endl;
}
}


namespace AEngine {

Engine::Engine() 
    : m_window(nullptr)  
    , m_gl_context(nullptr)
    , m_is_running(false)
    , m_renderer(nullptr)
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

    // สร้างและ init renderer
    m_renderer = new Renderer();
    m_renderer->init(m_window);    

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
        // สั่งให้ Renderer วาด 1 frame
        m_renderer->render();
    }
}

void Engine::shutdown() {
    std::cout << "A-Engine is shutting down." << std::endl;

    // สั่งให้ renderer คืนทรัพยากร และลบ instance ทิ้ง
    if (m_renderer) {
        m_renderer->shutdown();
        delete m_renderer;
        m_renderer = nullptr;
    }

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

} // namespace AEngine