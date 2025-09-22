#pragma once

namespace AEngine {
class World; // Forward declaration

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void init() = 0;
    virtual void update(World& world, float dt) = 0;
    virtual void shutdown() = 0;
};

} // namespace AEngine