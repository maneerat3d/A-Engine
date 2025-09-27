// #include "editor/editor.h"

// int main(int argc, char** argv) {

//     // สร้าง Editor Application
//     AEngine::Editor editor;
//     // สั่งให้ Editor ทำงาน
//     editor.run();

//     return 0;
// }

#include "editor/editor.h"
#include "core/plugin/iplugin.h"
#include "core/memory/default_allocator.h"
#include "core/memory/unique_ptr.h"

// --- เพิ่มเข้ามาเพื่อโหลด DLL บน Windows ---
#include <Windows.h>
#include <vector>
#include <string>


// ประกาศฟังก์ชันที่เราจะดึงมาจาก DLL
using CreatePluginFunc = AEngine::IPlugin* (*)(AEngine::IAllocator&);


int main(int argc, char** argv) {

    printf("Engine starting... Attempting to load plugins.\n");

    // สร้าง Allocator กลาง
    AEngine::DefaultAllocator mainAllocator;

    // สร้าง Array เพื่อเก็บ Plugin ที่โหลดสำเร็จ
    // สมมติว่ามี Array<T> หรือใช้ std::vector ก็ได้สำหรับ POC
    std::vector<AEngine::UniquePtr<AEngine::IPlugin>> plugins;

    // ชื่อของ DLL ที่เราจะโหลด
    const char* dllPath = "TestLogPlugin.dll";

    // โหลด Library
    HINSTANCE dllHandle = LoadLibraryA(dllPath);

    if (dllHandle) {
        printf("Successfully loaded library: %s\n", dllPath);

        // ค้นหาฟังก์ชัน createPlugin
        CreatePluginFunc createPlugin = (CreatePluginFunc)GetProcAddress(dllHandle, "createPlugin");

        if (createPlugin) {
            printf("Found 'createPlugin' function in DLL.\n");

            // เรียก Factory function เพื่อสร้าง Instance ของ Plugin
            AEngine::IPlugin* rawPlugin = createPlugin(mainAllocator);

            // ห่อหุ้มด้วย UniquePtr ของเอนจิ้นเพื่อจัดการ Memory
            AEngine::UniquePtr<AEngine::IPlugin> pluginPtr(rawPlugin, &mainAllocator);

            printf("Plugin GetName : '%s'\n", pluginPtr->getName());

            // เรียกใช้ฟังก์ชันของ Plugin เพื่อทดสอบ
            pluginPtr->createSystems(*(AEngine::Engine*)nullptr); // ส่ง nullptr ไปก่อนสำหรับ POC

            // ย้าย Ownership เข้าไปเก็บใน list
            plugins.push_back(std::move(pluginPtr));
        } else {
            printf("ERROR: Could not find 'createPlugin' function in %s\n", dllPath);
        }

    } else {
        printf("ERROR: Could not load library: %s. Error code: %lu\n", dllPath, GetLastError());
    }

    printf("\n--- Main application logic would run here ---\n\n");

    // สร้าง Editor Application
    AEngine::Editor editor;

    editor.run();

    // ตอนจบโปรแกรม, UniquePtr ใน vector จะถูกทำลาย
    // และจะเรียก destroySystems ของ plugin โดยอัตโนมัติ
    // เราสามารถเรียกเองเพื่อดู log ก็ได้
    for(auto& p : plugins) {
        p->destroySystems(*(AEngine::Engine*)nullptr);
    }
    plugins.clear(); // UniquePtrs will be destroyed here

    printf("Engine shutting down.\n");

    return 0;
}