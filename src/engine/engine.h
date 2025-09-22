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
    class IAllocator;
}
#include "core/container/array.h" // เพิ่ม
#include "core/memory/unique_ptr.h" // เพิ่ม
#include <unordered_map> // <--- เพิ่มบรรทัดนี้
#include <typeindex>     // <--- เพิ่มบรรทัดนี้
#include "core/string/string.h"

namespace AEngine {

class ISubsystem;
class IPlugin; // <--- เพิ่ม
class ISystem; // <--- เพิ่ม


class Engine {
public:
    Engine();
    explicit Engine(IAllocator& allocator); // แก้ไข Constructor
    ~Engine();

    void run();

    // --- Subsystem Management ---
    template <typename T>
    T* getSubsystem(const AEngine::String& name) const;
    template <typename T>
    T* getSubsystem() const;

    IAllocator& getAllocator() { return m_allocator; } // เพิ่ม Getter

    void addSystem(ISystem* system); // <--- เพิ่ม

private:
    void loadPlugins(); // <--- เพิ่ม
    bool init();
    void mainLoop();
    void shutdown();

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

} // namespace AEngine