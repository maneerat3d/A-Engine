#pragma once
#include "plugin/iplugin.h"

namespace AEngine {

class RendererPlugin : public IPlugin {
public:
    const char* getName() const override { return "Renderer"; }
    void createSystems(Engine& engine) override;
    void destroySystems(Engine& engine) override;
};

} // namespace AEngine