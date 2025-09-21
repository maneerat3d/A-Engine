#pragma once

namespace AEngine {
class Scene; // Forward declaration

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void init() = 0;
    virtual void update(Scene& scene, float dt) = 0;
    virtual void shutdown() = 0;
};

} // namespace AEngine