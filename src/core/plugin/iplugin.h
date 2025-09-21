#pragma once
#include <vector>

namespace AEngine {
class ISystem;
class Engine;

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual const char* getName() const = 0;
    virtual void createSystems(Engine& engine) = 0;
    virtual void destroySystems(Engine& engine) = 0;
};

} // namespace AEngine