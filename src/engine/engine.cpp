#include "engine.h"
#include "core/plugin/iplugin.h"
#include "core/ecs/isystem.h"
#include "core/scene/scene.h"
#include "core/resource/resource_manager.h" // Engine ยังคงต้องรู้จัก ResourceManager เพื่อส่งต่อให้ Plugin

// --- Include แค่ตัว Plugin ไม่ใช่รายละเอียดข้างใน ---
#include "renderer/renderer_plugin.h"
#include "plugins/gltf_importer/gltf_importer_plugin.h" // เพิ่ม include นี้
#include "game_plugin.h"

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
    , m_scene(nullptr)
{
}

Engine::~Engine() {
    // Destructor ควรจะว่างเปล่า, การจัดการทรัพยากรควรอยู่ใน shutdown()
}

void Engine::run() {

    if (!init()) {
        std::cerr << "FATAL: Engine failed to initialize. Shutting down." << std::endl;
        shutdown();
        return;
    }
    loadPlugins();
    gameLoop();
    shutdown();
}

bool Engine::init() {
   std::cout << "A-Engine is initializing..." << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_sdl_error("SDL_Init");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_window = SDL_CreateWindow("A-Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
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

    // สร้าง Scene และ ResourceManager ที่นี่
    m_scene = new Scene();
    m_resourceManager = new ResourceManager();

    m_scene->getCamera().setPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    m_scene->getCamera().setPosition({0.0f, 2.0f, 5.0f});
    m_scene->getCamera().lookAt({0.0f, 0.0f, 0.0f});
    return true;
}

void Engine::loadPlugins() {
    std::cout << "Loading plugins..." << std::endl;

    // --- Renderer Plugin ---
    m_plugins.push_back(new RendererPlugin(*this));
    // --- GLTF Importer Plugin ---
    m_plugins.push_back(new GltfImporterPlugin(*this));
    // --- Game Plugin ---
    m_plugins.push_back(new GamePlugin(*this));
    
    std::cout << "--- All plugins constructed. Now calling createSystems... ---" << std::endl; // <-- ปักธงที่ 1
    // Create all plugins (which also registers systems and importers)
    for (auto* plugin : m_plugins) {
        std::cout << "Calling createSystems for plugin: " << plugin->getName() << std::endl; // <-- ปักธงที่ 2
        plugin->createSystems(*this); 
        std::cout << "Finished createSystems for plugin: " << plugin->getName() << std::endl; // <-- ปักธงที่ 3
    }

    // CRITICAL: เพิ่มส่วนที่ขาดหายไปกลับเข้ามา!
    std::cout << "--- Initializing all systems... ---" << std::endl;
    for (auto* system : m_systems) {
        system->init();
    }

    std::cout << "--- Finished loading all plugins. ---" << std::endl; // <-- ปักธงที่ 4
    m_is_running = true;
}

void Engine::gameLoop() {
    std::cout << "--- ENTERING GAME LOOP ---" << std::endl; // <-- ปักธงที่ 5

    // เพิ่มบรรทัดนี้เพื่อดูค่าของ m_is_running ก่อนเข้า Loop
    std::cout << "Value of m_is_running before loop: " << (m_is_running ? "true" : "false") << std::endl;

    uint32_t last_tick = SDL_GetTicks();

    while (m_is_running) {
        uint32_t current_tick = SDL_GetTicks();
        float dt = (current_tick - last_tick) / 1000.0f;
        last_tick = current_tick;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_is_running = false;
            }
        }
        // --- Update all registered systems ---
        for (auto* system : m_systems) {
            system->update(*m_scene, dt);
        }
        // การ Swap Window ควรเป็นหน้าที่ของ RenderSystem ไม่ใช่ Engine
        // แต่จากโค้ดเดิม RenderSystem ไม่ได้ทำ ดังนั้นเราต้องย้าย SDL_GL_SwapWindow() ไปไว้ใน renderer.cpp
        // หรือปล่อยไว้ที่นี่ก่อนเพื่อแก้ปัญหาเฉพาะหน้า (ซึ่งโค้ด error ของคุณทำไว้ที่นี่แล้ว)
    }
}

void Engine::shutdown() {
    std::cout << "A-Engine is shutting down." << std::endl;

    for (auto* system : m_systems) {
        system->shutdown();
        delete system;
    }
    m_systems.clear();

    for (auto* plugin : m_plugins) {
        plugin->destroySystems(*this);
        delete plugin;
     }

    m_plugins.clear();

    if (m_resourceManager) {
        delete m_resourceManager;
        m_resourceManager = nullptr;
    }    
    if (m_scene) {
        delete m_scene;
        m_scene = nullptr;
    }
    
    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

} // namespace AEngine