#include "platform_subsystem.h"
#include "engine/engine.h" // แม้จะดูแปลก แต่จำเป็นสำหรับ Logging และ Error Handling
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

namespace {
    void log_sdl_error(const char* msg) {
        std::cerr << msg << ": " << SDL_GetError() << std::endl;
    }
}

namespace AEngine {

PlatformSubsystem::PlatformSubsystem(Engine& context)
    : ISubsystem(context, "PlatformSubsystem"), m_window(nullptr), m_gl_context(nullptr), m_quit_requested(false)
{
}

PlatformSubsystem::~PlatformSubsystem()
{
}

bool PlatformSubsystem::init() {
    std::cout << "PlatformSubsystem is initializing..." << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_sdl_error("SDL_Init");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_window = SDL_CreateWindow("A-Engine (Refactored)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    if (!m_window) {
        log_sdl_error("SDL_CreateWindow");
        SDL_Quit();
        return false;
    }

    m_gl_context = SDL_GL_CreateContext(m_window);
    if (!m_gl_context) {
        log_sdl_error("SDL_GL_CreateContext");
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_GL_DeleteContext(m_gl_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    return true;
}

void PlatformSubsystem::update(float dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_quit_requested = true;
        }
    }
     // การ Swap Window จะถูกย้ายไปที่ RenderSystem ในอนาคต แต่ตอนนี้ให้ Platform ทำไปก่อน
    SDL_GL_SwapWindow(m_window);
}

void PlatformSubsystem::shutdown() {
    std::cout << "PlatformSubsystem is shutting down." << std::endl;
    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

} // namespace AEngine