#pragma once
#include "core/plugin/iplugin.h"

class Engine;
namespace AEngine {
   
class GamePlugin : public IPlugin {
public:
    explicit GamePlugin(Engine& engine);
    const char* getName() const override { return "Game"; }
    void createSystems(Engine& engine) override;
    void destroySystems(Engine& engine) override;
    ~GamePlugin() override;
private:
    Engine& m_engine;
};

} // namespace AEngine