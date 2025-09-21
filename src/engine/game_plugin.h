#pragma once
#include "core/plugin/iplugin.h"

namespace AEngine {

class GamePlugin : public IPlugin {
public:
    const char* getName() const override { return "Game"; }
    void createSystems(Engine& engine) override;
    void destroySystems(Engine& engine) override;
};

} // namespace AEngine