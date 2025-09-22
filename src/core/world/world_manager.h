#pragma once

#include "core/subsystem/isubsystem.h"
#include "core/memory/unique_ptr.h"

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
    UniquePtr<World> m_active_world;
};

} // namespace AEngine