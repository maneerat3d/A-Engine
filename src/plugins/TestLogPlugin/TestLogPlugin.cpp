 #include "TestLogPlugin.h"
 #include "core/memory/allocator.h" // สมมติว่า Allocator อยู่ที่นี่
 
 namespace AEngine {
 
 const char* TestLogPlugin::getName() const {
     return "Test Log Plugin";
 }
 
 void TestLogPlugin::createSystems(Engine& engine) {
     // นี่คือส่วนที่ปลั๊กอินจะทำงานเมื่อถูกโหลด!
     printf("=======================================\n");
     printf(">>> TestLogPlugin Loaded & Initialized! <<<\n");
     printf("=======================================\n");
 }
 
 void TestLogPlugin::destroySystems(Engine& engine) {
     printf(">>> TestLogPlugin Destroyed! <<<\n");
 }
 
 // --- Factory Function (หัวใจสำคัญ) ---
 // ฟังก์ชันนี้จะถูก export ออกจาก DLL เพื่อให้ Engine.exe เรียกใช้
 // extern "C" เพื่อป้องกัน C++ Name Mangling
 // __declspec(dllexport) คือคำสั่งของ MSVC Compiler เพื่อบอกว่าให้ export ฟังก์ชันนี้
 extern "C" __declspec(dllexport) IPlugin* createPlugin(IAllocator& allocator) {
     // สร้าง instance ของ Plugin โดยใช้ Allocator ที่ได้รับมาจากเอนจิ้น
     return AENGINE_NEW(allocator, TestLogPlugin)();
 }
 
 } // namespace AEngine