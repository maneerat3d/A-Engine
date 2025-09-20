#include <iostream>
// ดึง Header ของ SDL เข้ามาใช้งาน
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h> // ดึง Header สำหรับ OpenGL เข้ามา

// ฟังก์ชันสำหรับจัดการข้อผิดพลาดของ SDL
void log_sdl_error(const char* msg) {
    std::cerr << msg << ": " << SDL_GetError() << std::endl;
}
 
int main() {
    std::cout << "A-Engine is starting..." << std::endl;

    // 1. เริ่มต้นการทำงานของระบบ SDL (ในที่นี้คือระบบ Video)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_sdl_error("Failed to initialize SDL");
        return -1;
    }

    // === ตั้งค่าเวอร์ชั่นของ OpenGL ที่เราต้องการ ===
    // เราจะใช้ OpenGL 3.3 ซึ่งเป็นเวอร์ชั่นที่ทันสมัยและรองรับได้กว้าง
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // ใช้ Core Profile คือไม่ใช้ฟังก์ชันเก่าๆ ของ OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // เปิดใช้งาน Double Buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    
    // 2. สร้างหน้าต่าง
    SDL_Window* window = SDL_CreateWindow(
        "A-Engine",                 // Title ของหน้าต่าง
        SDL_WINDOWPOS_CENTERED,     // ตำแหน่ง X (กึ่งกลาง)
        SDL_WINDOWPOS_CENTERED,     // ตำแหน่ง Y (กึ่งกลาง)
        1280,                       // ความกว้าง
        720,                        // ความสูง
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // Flag เพิ่มเติม
    );

    if (!window) {
        log_sdl_error("Failed to create window");
        SDL_Quit();
        return -1;
    }

    // 3. สร้าง OpenGL Context จาก Window
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        log_sdl_error("Failed to create OpenGL context");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool is_running = true;
    SDL_Event event;

    // นี่คือ Game Loop ของเรา!
    while (is_running) {
        
        // 1. จัดการ Input (Event Pumping)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }
        // 2. Update (ยังไม่มี)

        // 3. Render
        // 3.1 ตั้งค่าสีที่จะใช้ล้างหน้าจอ (RGBA) - สีน้ำเงิน
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        // 3.2 สั่งให้ GPU ล้างหน้าจอด้วยสีที่ตั้งไว้
        glClear(GL_COLOR_BUFFER_BIT);

        // 3.3 สลับ Buffer (นำภาพที่วาดเสร็จแล้วใน Back Buffer ไปแสดงผล)
        SDL_GL_SwapWindow(window);
    }

    // 5. คืนทรัพยากรเมื่อจบโปรแกรม
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "A-Engine has shut down." << std::endl;
    return 0;
}