#include <engine.h>
#include "core/memory/default_allocator.h"

int main(int argc, char** argv) {

    // สร้าง Allocator หลักสำหรับใช้งาน
    AEngine::DefaultAllocator allocator;    

    // ส่ง Allocator เข้าไปใน constructor
    AEngine::Engine engine(allocator);     
    // สั่งให้ Engine ทำงาน
    engine.run();

    return 0;
}