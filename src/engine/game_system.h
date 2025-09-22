#pragma once
#include "core/ecs/isystem.h"

namespace AEngine {

class GameSystem : public ISystem {
public:
    GameSystem() = default;
    ~GameSystem() = default;

    void init() override;
    void update(World& world, float dt) override;
    void shutdown() override;
};

} // namespace AEngine