#pragma once
#include "plugin/iplugin.h"

namespace AEngine {
class Engine; 
 class RendererPlugin final : public IPlugin {
public:

    explicit RendererPlugin(Engine& engine);
    const char* getName() const override { return "Renderer"; }
    void createSystems(Engine& engine) override;
    void destroySystems(Engine& engine) override;
private:
    Engine& m_engine; // <--- เพิ่ม member นี้    
};

} // namespace AEngine