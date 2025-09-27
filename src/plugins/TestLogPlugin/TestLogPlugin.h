 #pragma once
 #include "core/plugin/IPlugin.h"
 #include <cstdio> // หรือ <iostream>
 
 namespace AEngine {
 
 class TestLogPlugin : public IPlugin {
 public:
     const char* getName() const override;
     void createSystems(Engine& engine) override;
     void destroySystems(Engine& engine) override;
 };
 
 } // namespace AEngine