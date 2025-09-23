#pragma once
#include "core/core.h"
#include "core/memory/unique_ptr.h"

namespace AEngine {
    class Framebuffer;
    class Engine;

    class SceneView {
    public:
        SceneView(Engine* engine);
        ~SceneView();

        void onGUI();

    private:
        Engine* m_engine;
        UniquePtr<Framebuffer> m_framebuffer;
        u32 m_width = 1024, m_height = 1024;
    };
}