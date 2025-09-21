#include "engine.h"
#include "scene/scene.h"
#include "resource/resource_manager.h" // เพิ่ม resource manager
#include "renderer/mesh.h" // เพิ่ม mesh และ texture
#include "renderer/texture.h"
#include "renderer/gltf_importer.h"
#include "core/plugin/iplugin.h"
#include "core/ecs/isystem.h"

// --- Include Plugins ---
#include "renderer/renderer_plugin.h"
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
    init();
    loadPlugins();
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

    // สร้าง ResourceManager ก่อน
    m_resourceManager = new ResourceManager();    
    // Scene creation is now simpler
    m_scene = new Scene();
    m_scene->getCamera().setPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    m_scene->getCamera().setPosition({0.0f, 2.0f, 5.0f});
    m_scene->getCamera().lookAt({0.0f, 0.0f, 0.0f});
}

void Engine::loadPlugins() {
    // === เตรียมข้อมูล Vertex ของลูกบาศก์ (ย้ายมาจาก renderer) ===

    auto containerTex = m_resourceManager->load<Texture>("textures/container.jpg");

    // Load the model using our new importer
    auto loadedMeshes = GltfImporter::load("models/monkey.glb", *m_resourceManager);

    // Statically load plugins for now
    m_plugins.push_back(new RendererPlugin());
    m_plugins.push_back(new GamePlugin());

    for (auto* plugin : m_plugins) {
        plugin->createSystems(*this);
    }

    for (auto* system : m_systems) {
        system->init();
    }
    
    // --- Create Entities from the loaded model ---
    if (!loadedMeshes.empty()) {
        Entity model1 = m_scene->createEntity();
        {
            ECS::TransformComponent transform;
            transform.position = {-1.5f, 0.0f, 0.0f};
            m_scene->addComponent(model1, transform);

            // Use the first mesh from the loaded model
            m_scene->addComponent<ECS::RenderableComponent>(model1, {loadedMeshes[0], containerTex});
            m_scene->addComponent<ECS::RotatingCubeComponent>(model1, {1.0f});
        }
        Entity model2 = m_scene->createEntity();
        {
            ECS::TransformComponent transform;
            transform.position = {1.5f, 0.0f, 0.0f};
            transform.scale = {0.5f, 0.5f, 0.5f};
            m_scene->addComponent(model2, transform);

            m_scene->addComponent<ECS::RenderableComponent>(model2, {loadedMeshes[0], containerTex});
        }
     }
     
    m_is_running = true;
}

void Engine::gameLoop() {

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