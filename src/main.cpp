#include <iostream>
#include <chrono>   // สำหรับจัดการเรื่องเวลา
#include <thread>   // สำหรับการหน่วงเวลา (sleep)


int main() {
    std::cout << "A-Engine is starting..." << std::endl;

    bool is_running = true;
    int frame_count = 0;

    // นี่คือ Game Loop ของเรา!
    while (is_running) {
        std::cout << "--- Frame " << ++frame_count << " ---" << std::endl;

        // 1. Process Input (จะเพิ่มโค้ดจริงในอนาคต)
        std::cout << "Processing Input..." << std::endl;

        // 2. Update (จะเพิ่มโค้ดจริงในอนาคต)
        std::cout << "Updating Game State..." << std::endl;

        // 3. Render (จะเพิ่มโค้ดจริงในอนาคต)
        std::cout << "Rendering..." << std::endl;

        // หน่วงเวลาเล็กน้อยเพื่อไม่ให้ Console ทำงานเร็วเกินไป
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}