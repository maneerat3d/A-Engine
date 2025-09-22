#pragma once

// -- PHASE 1: Decoupling the Core Engine --
// Step 1.3: Elevate ResourceManager and World
// WorldManager ทำหน้าที่จัดการ World ทั้งหมด (ในอนาคตอาจมีหลาย World)
// ทำให้ระบบอื่นๆ สามารถเข้าถึง World ปัจจุบันได้ผ่าน Subsystem นี้

#include "core/subsystem/isubsystem.h"
#include <memory>

namespace AEngine {

class World;

class WorldManager final : public ISubsystem {
public:
    WorldManager(Engine& context);
    ~WorldManager();

    bool init() override;
    void update(float dt) override;
    void shutdown() override;

    World* getActiveWorld() const { return m_active_world.get(); }

private:
    std::unique_ptr<World> m_active_world;
};

} // namespace AEngine