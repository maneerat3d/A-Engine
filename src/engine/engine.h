#pragma once

// Forward declaration เพื่อไม่ต้อง include SDL.h ใน header
struct SDL_Window;
using SDL_GLContext = void*; // Forward declare GL Context

// Forward declare Renderer เพื่อไม่ต้อง include renderer.h
namespace AEngine {
    class Scene;
    class ResourceManager;
    class IPlugin;
    class ISystem;
}

#include <vector> 
namespace AEngine {

class Engine {
public:
    Engine();
    ~Engine();

    void run();

private:
    bool init();
    void loadPlugins();
    void gameLoop();
    void shutdown();

    // Member variables สำหรับเก็บสถานะของ Engine
    SDL_Window* m_window; // Engine ยังคงเป็นเจ้าของ window
    SDL_GLContext m_gl_context; // และ GL Context
    bool m_is_running;
    Scene* m_scene;       // Engine เป็นเจ้าของ Scene
    ResourceManager* m_resourceManager; // Engine เป็นเจ้าของ ResourceManager

    // Plugin and System Management
    std::vector<IPlugin*> m_plugins;
    std::vector<ISystem*> m_systems;

public:
    // Public getters for plugins to access necessary resources
    SDL_Window* getWindow() { return m_window; }
    ResourceManager* getResourceManager() { return m_resourceManager; }
    void addSystem(ISystem* system) { m_systems.push_back(system); }    
};

} // namespace AEngine