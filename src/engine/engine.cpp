#include "engine.h"
#include "renderer.h"
#include "scene/scene.h"
#include "resource/resource_manager.h" // เพิ่ม resource manager
#include "renderer/mesh.h" // เพิ่ม mesh และ texture
#include "renderer/texture.h"
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
    , m_resourceManager(nullptr)
    , m_scene(nullptr)
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

    // สร้าง ResourceManager ก่อน
    m_resourceManager = new ResourceManager();    
    // สร้างและ init renderer
    m_renderer = new Renderer();
    m_renderer->init(m_window);    

    // === เตรียมข้อมูล Vertex ของลูกบาศก์ (ย้ายมาจาก renderer) ===
    std::vector<float> cubeVertices = {
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

    // สร้าง Mesh และ Texture ผ่าน ResourceManager
    auto cubeMesh = m_resourceManager->load<Mesh>("builtin_cube", cubeVertices);
    auto containerTex = m_resourceManager->load<Texture>("textures/container.jpg");


    // สร้างและตั้งค่า Scene
    m_scene = new Scene();
    m_scene->getCamera().setPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    m_scene->getCamera().setPosition({0.0f, 2.0f, 5.0f});
    m_scene->getCamera().lookAt({0.0f, 0.0f, 0.0f});

    // สร้าง GameObject 2 ชิ้น
    auto& cube1 = m_scene->createGameObject("Cube 1");
    cube1.mesh = cubeMesh;
    cube1.texture = containerTex;    
    cube1.transform.position = {-1.0f, 0.0f, 0.0f};

    auto& cube2 = m_scene->createGameObject("Cube 2");
    cube2.mesh = cubeMesh;
    cube2.texture = containerTex;    
    cube2.transform.position = {1.0f, 0.0f, 0.0f};
    cube2.transform.scale = {0.5f, 0.5f, 0.5f};

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
        // อัปเดต GameObject (ตัวอย่างการทำให้หมุน)
        float time = (float)SDL_GetTicks() / 1000.0f;
        auto& gameObjects = const_cast<std::vector<AEngine::GameObject>&>(m_scene->getGameObjects());
        gameObjects[0].transform.rotation = glm::angleAxis(time, glm::vec3(0.0f, 1.0f, 0.0f));
        gameObjects[1].transform.rotation = glm::angleAxis(time * 0.5f, glm::vec3(0.5f, 1.0f, 0.0f));
    
        // สั่งให้ Renderer วาด 1 frame
        m_renderer->render(*m_scene);
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