#pragma once

// -- PHASE 1: Decoupling the Core Engine --
// Step 1.2: Create Platform Abstraction Layer
// PlatformSubsystem ทำหน้าที่แยกโค้ดที่เกี่ยวข้องกับ OS และ Library (เช่น SDL) ออกจาก Engine Core
// ทำให้สามารถเปลี่ยนไปใช้ GLFW หรือ Backend อื่นๆ ได้ในอนาคตโดยกระทบส่วนอื่นน้อยที่สุด

#include "core/subsystem/isubsystem.h"

// Forward declaration
struct SDL_Window;
using SDL_GLContext = void*;

namespace AEngine {

class PlatformSubsystem final : public ISubsystem {
public:
    PlatformSubsystem(Engine& context);
    ~PlatformSubsystem();

    bool init() override;
    void update(float dt) override;
    void shutdown() override;

    SDL_Window* getWindow() const { return m_window; }
    bool isQuitRequested() const { return m_quit_requested; }

private:
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    bool m_quit_requested;
};

} // namespace AEngine