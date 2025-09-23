#pragma once
struct SDL_Window;
using SDL_GLContext = void*; // Forward declare GL Context

// Forward declare Renderer เพื่อไม่ต้อง include renderer.h
namespace AEngine {
    class Scene;
    class ResourceManager;
    class IPlugin;
    class ISystem;
    class IAllocator;
}
#include "core/container/array.h" 
#include "core/memory/unique_ptr.h" 
#include <unordered_map> 
#include <typeindex>     
#include "core/string/string.h"

namespace AEngine {

class ISubsystem;
class IPlugin; 
class ISystem; 


class Engine {
public:
    Engine();
    explicit Engine(IAllocator& allocator); // แก้ไข Constructor
    ~Engine();

    bool init();
    void update(float dt);
    void shutdown();
    

    // --- Subsystem Management ---
    template <typename T>
    T* getSubsystem(const AEngine::String& name) const;
    template <typename T>
    T* getSubsystem() const;

    IAllocator& getAllocator() { return m_allocator; } // เพิ่ม Getter

    void addSystem(ISystem* system); 
    const Array<ISystem*>& getSystems() const { return m_systems; }

    template <typename T>
    T* getSystem() const;

private:
    void loadPlugins(); 


    void createSubsystems();

    IAllocator& m_allocator;

    bool m_is_running;

    Array<UniquePtr<ISubsystem>> m_subsystems;
    std::unordered_map<std::type_index, ISubsystem*> m_subsystem_map;
    // --- Plugin and System Management ---
    Array<UniquePtr<IPlugin>> m_plugins;
    Array<ISystem*> m_systems;  
};    

template <typename T>
T* Engine::getSubsystem() const {
    auto it = m_subsystem_map.find(std::type_index(typeid(T)));
    if (it != m_subsystem_map.end()) {
        return static_cast<T*>(it->second);
    }
    return nullptr;
} 

template <typename T>
T* Engine::getSystem() const {
    for (auto* system : m_systems) {
        if (auto* result = dynamic_cast<T*>(system)) {
            return result;
        }
    }
    return nullptr;
}


} // namespace AEngine