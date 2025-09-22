#include "engine.h"

// ... includes subsystems ...
#include "core/plugin/iplugin.h" // <--- เพิ่ม
#include "core/ecs/isystem.h"    // <--- เพิ่ม
#include "core/world/world.h"      // <--- เพิ่ม

// --- Include Plugins ---
#include "renderer/renderer_plugin.h"
#include "plugins/gltf_importer/gltf_importer_plugin.h"
#include "game_plugin.h"

#include "core/subsystem/isubsystem.h"
#include "platform/platform_subsystem.h" // <--- Include Subsystem ใหม่
#include "core/resource/resource_manager.h"
#include "core/world/world_manager.h" // <--- Include Subsystem ใหม่
#include "core/memory/unique_ptr.h"

#include <iostream>
#include <SDL.h> // ยังต้องใช้สำหรับ SDL_GetTicks()

namespace AEngine {

Engine::Engine(IAllocator& allocator)
    : m_allocator(allocator)
    , m_is_running(false)
    , m_subsystems(allocator)
    , m_plugins(allocator)
    , m_systems(allocator)
 {
 }

Engine::~Engine() {
}

void Engine::createSubsystems()
{
    // เรียงลำดับการสร้าง Subsystem ตามความสำคัญ
    // Platform ต้องถูกสร้างก่อน เพราะระบบอื่นอาจต้องใช้ Window
    m_subsystems.emplace(UniquePtr<PlatformSubsystem>::create(m_allocator, *this));
    m_subsystems.emplace(UniquePtr<ResourceManager>::create(m_allocator, *this));
    m_subsystems.emplace(UniquePtr<WorldManager>::create(m_allocator, *this));

    // สร้าง Map เพื่อให้ค้นหา Subsystem ตาม Type ได้เร็ว
    for (const auto& sub : m_subsystems)
    {
        // หมายเหตุ: การใช้ dynamic_cast ที่นี่อาจไม่สวยงามที่สุด แต่เป็นวิธีที่ง่ายในการเริ่มต้น
        if (auto* p = dynamic_cast<PlatformSubsystem*>(sub.get())) m_subsystem_map[typeid(PlatformSubsystem)] = p;
        if (auto* r = dynamic_cast<ResourceManager*>(sub.get())) m_subsystem_map[typeid(ResourceManager)] = r;
        if (auto* w = dynamic_cast<WorldManager*>(sub.get())) m_subsystem_map[typeid(WorldManager)] = w;
    }
}


void Engine::run() {
    if (!init()) {
        std::cerr << "FATAL: Engine failed to initialize. Shutting down." << std::endl;
        shutdown();
        return;
    }
    mainLoop();
    shutdown();
}

bool Engine::init() {
    std::cout << "A-Engine is initializing..." << std::endl;
    createSubsystems();

    for (const auto& subsystem : m_subsystems) {
        if (!subsystem->init()) {
            std::cerr << "FATAL: Subsystem " << subsystem->getName().c_str() << " failed to initialize." << std::endl;
            return false;
        }
    }

    loadPlugins(); // <--- เรียกใช้ loadPlugins ที่นี่

    // Initializing all systems that were added by plugins
    std::cout << "--- Initializing all systems... ---" << std::endl;
    for (auto* system : m_systems) {
        system->init();
    }

    m_is_running = true;
    return true;
}

// --- ฟังก์ชันใหม่ ---
void Engine::loadPlugins() {
    std::cout << "Loading plugins..." << std::endl;
    // 1. ใช้ UniquePtr::create และส่ง m_allocator เข้าไป
    // 2. ใช้ .emplace() ของ Array เพื่อสร้าง UniquePtr ลงใน Array โดยตรง
    m_plugins.emplace(UniquePtr<RendererPlugin>::create(m_allocator, *this));
    m_plugins.emplace(UniquePtr<GltfImporterPlugin>::create(m_allocator, *this));
    m_plugins.emplace(UniquePtr<GamePlugin>::create(m_allocator, *this));

    std::cout << "--- All plugins constructed. Now calling createSystems... ---" << std::endl;
    // 3. ส่วนของ for-loop สามารถใช้งานได้เหมือนเดิม เพราะ Array ของเรามี begin() และ end()
    for (const auto& plugin : m_plugins) {
        std::cout << "Calling createSystems for plugin: " << plugin->getName() << std::endl;
        plugin->createSystems(*this);
        std::cout << "Finished createSystems for plugin: " << plugin->getName() << std::endl;
    }
}
// --- ฟังก์ชันใหม่ ---
void Engine::addSystem(ISystem* system) {
    m_systems.push(system);
}

void Engine::mainLoop() {
    std::cout << "--- ENTERING MAIN LOOP ---" << std::endl;
    uint32_t last_tick = SDL_GetTicks();

    auto* platform = getSubsystem<PlatformSubsystem>();

   // --- DIAGNOSTIC: เพิ่มบรรทัดนี้เพื่อตรวจสอบค่าของ m_is_running ---
   std::cout << "Value of m_is_running before loop: " << (m_is_running ? "true" : "false") << std::endl;

    while (m_is_running) {
        if (platform && platform->isQuitRequested()) {
            m_is_running = false;
            continue;
        }

        uint32_t current_tick = SDL_GetTicks();
        float dt = (current_tick - last_tick) / 1000.0f;
        last_tick = current_tick;

        // --- Update all subsystems ---
        for (const auto& subsystem : m_subsystems) {
            subsystem->update(dt);
        }
        // --- Update all systems ---
        WorldManager* worldManager = getSubsystem<WorldManager>();
        if (worldManager) {
            World* world = worldManager->getActiveWorld();
            if (world) {
                // วนลูปเพื่อเรียก update ของทุก System ที่ลงทะเบียนไว้
                for (auto* system : m_systems) {
                    system->update(*world, dt);
                }
            }
        }
    }
}

void Engine::shutdown() {
    std::cout << "A-Engine is shutting down." << std::endl;

    for (auto* system : m_systems) {
        system->shutdown();
        AENGINE_DELETE(m_allocator, system);
    }
    m_systems.clear();

    for (auto& plugin : m_plugins) {
        plugin->destroySystems(*this);
    }
    m_plugins.clear();

    // Shutdown subsystems ในลำดับย้อนกลับ
    for (i32 i = m_subsystems.size() - 1; i >= 0; --i) {
        m_subsystems[i]->shutdown();
    }
    m_subsystems.clear();
}

} // namespace AEngine