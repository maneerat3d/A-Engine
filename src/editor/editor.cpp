#include "editor.h"
#include "scene_view.h"
#include "engine/engine.h"
#include "core/memory/default_allocator.h"
#include "platform/platform_subsystem.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <glad/glad.h>
#include <SDL.h>
#include <iostream>

namespace AEngine {

Editor::Editor()
    : m_allocator(nullptr), m_engine(nullptr), m_scene_view(nullptr), m_is_running(false)
{
}

Editor::~Editor()
{
}

void Editor::init() {
    m_allocator = new DefaultAllocator();
    m_engine = new Engine(*m_allocator);
    
    m_engine->init(); // init engine and its subsystems

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    PlatformSubsystem* platform = m_engine->getSubsystem<PlatformSubsystem>();
    ImGui_ImplSDL2_InitForOpenGL(platform->getWindow(), platform->getGLContext()); 
    ImGui_ImplOpenGL3_Init("#version 330 core"); 

    m_scene_view = new SceneView(m_engine);
}

void Editor::shutdown() {
    delete m_scene_view;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    m_engine->shutdown();
    delete m_engine;
    delete m_allocator;
}

void Editor::run() {
    init();
    mainLoop();
    shutdown();
}

void Editor::mainLoop() {
    m_is_running = true;
    PlatformSubsystem* platform = m_engine->getSubsystem<PlatformSubsystem>();

    u64 NOW = SDL_GetPerformanceCounter();
    u64 LAST = 0;
    float dt = 0;

    while (m_is_running) {
        
        // --- คำนวณ Delta Time ในแต่ละ Frame ---
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        dt = (float)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001f;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                m_is_running = false;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(platform->getWindow())) {
                m_is_running = false;
            }
        }

        m_engine->update(dt);
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Render Editor UI
        renderUI();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        SDL_GL_SwapWindow(platform->getWindow());
    }
}

void Editor::renderUI() {
    // Main Dockspace
    static bool dockspace_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspace_open, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit")) { m_is_running = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Render different editor windows
    m_scene_view->onGUI();
    ImGui::ShowDemoWindow();

    ImGui::End();
}

} // namespace AEngine