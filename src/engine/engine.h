#pragma once
// -- PHASE 1: Decoupling the Core Engine --
// Engine ถูกปรับบทบาทเป็น Context หรือ Service Locator
// ไม่ได้เป็นเจ้าของระบบหลักๆ อีกต่อไป แต่ทำหน้าที่จัดการ Lifecycle
// ของ Subsystems และเป็นศูนย์กลางให้ระบบต่างๆ สื่อสารถึงกัน
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
#include <memory>
#include <string>
#include <unordered_map> // <--- เพิ่มบรรทัดนี้
#include <typeindex>     // <--- เพิ่มบรรทัดนี้

namespace AEngine {

class ISubsystem;
class IPlugin; // <--- เพิ่ม
class ISystem; // <--- เพิ่ม


class Engine {
public:
    Engine();
    ~Engine();

    void run();

    // --- Subsystem Management ---
    template <typename T>
    T* getSubsystem(const std::string& name) const;
    template <typename T>
    T* getSubsystem() const;

    void addSystem(ISystem* system); // <--- เพิ่ม

private:
    void loadPlugins(); // <--- เพิ่ม
    bool init();
    void mainLoop();
    void shutdown();

    void createSubsystems();

    bool m_is_running;

    std::vector<std::unique_ptr<ISubsystem>> m_subsystems;
    std::unordered_map<std::type_index, ISubsystem*> m_subsystem_map;
    // --- Plugin and System Management ---
    std::vector<std::unique_ptr<IPlugin>> m_plugins; // <--- เพิ่ม
    std::vector<ISystem*> m_systems;  
};    

template <typename T>
T* Engine::getSubsystem() const {
    auto it = m_subsystem_map.find(std::type_index(typeid(T)));
    if (it != m_subsystem_map.end()) {
        return static_cast<T*>(it->second);
    }
    return nullptr;
} 
    
//     void loadPlugins();
//     // Member variables สำหรับเก็บสถานะของ Engine
//     SDL_Window* m_window; // Engine ยังคงเป็นเจ้าของ window
//     SDL_GLContext m_gl_context; // และ GL Context
//     Scene* m_scene;       // Engine เป็นเจ้าของ Scene
//     ResourceManager* m_resourceManager; // Engine เป็นเจ้าของ ResourceManager

//     // Plugin and System Management
//     std::vector<IPlugin*> m_plugins;
//     std::vector<ISystem*> m_systems;

// public:
//     // Public getters for plugins to access necessary resources
//     SDL_Window* getWindow() { return m_window; }
//     Scene* getScene() { return m_scene; } // <-- เพิ่มบรรทัดนี้
//     ResourceManager* getResourceManager() { return m_resourceManager; } 
//     void addSystem(ISystem* system) { m_systems.push_back(system); }    
// };

} // namespace AEngine