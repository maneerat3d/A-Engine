#include <iostream>
// ดึง Header ของ SDL เข้ามาใช้งาน
#define SDL_MAIN_HANDLED
#include <SDL.h>

int main() {
    std::cout << "A-Engine is starting..." << std::endl;

    // 1. เริ่มต้นการทำงานของระบบ SDL (ในที่นี้คือระบบ Video)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // 2. สร้างหน้าต่าง
    SDL_Window* window = SDL_CreateWindow(
        "A-Engine",                 // Title ของหน้าต่าง
        SDL_WINDOWPOS_CENTERED,     // ตำแหน่ง X (กึ่งกลาง)
        SDL_WINDOWPOS_CENTERED,     // ตำแหน่ง Y (กึ่งกลาง)
        1280,                       // ความกว้าง
        720,                        // ความสูง
        SDL_WINDOW_SHOWN            // Flag: ให้แสดงหน้าต่าง
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
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
        // 3. Render (ยังไม่มี)
    }
    // 4. คืนทรัพยากรเมื่อจบโปรแกรม
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "A-Engine has shut down." << std::endl;
    return 0;
}