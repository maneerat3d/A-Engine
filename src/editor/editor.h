#pragma once

namespace AEngine {
    class Engine;
    class IAllocator;
    class EditorUI;
    class SceneView;
}

namespace AEngine {

class Editor {
public:
    Editor();
    ~Editor();

    void run();
    void setIsRunning(bool running) { m_is_running = running; }
    bool isRunning() const { return m_is_running; }

private:
    void init();
    void shutdown();

    void mainLoop();
    void renderUI();

    IAllocator* m_allocator;
    Engine* m_engine;
    EditorUI* m_editor_ui;
    SceneView* m_scene_view;

    bool m_is_running;
};

} // namespace AEngine