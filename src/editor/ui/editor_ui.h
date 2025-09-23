#pragma once
#include "core/string/string.h"
#include <string>

namespace AEngine {

class Engine;
class Editor; 

class EditorUI {
public:
    EditorUI(Editor& editor, Engine& engine, IAllocator& allocator);
    ~EditorUI();

    // --- ส่วนที่แก้ไข: แยกฟังก์ชันการวาด ---
    void drawMenuBarItems(); // สำหรับวาดเมนู File, Edit, etc.
    void drawPopups();       // สำหรับวาด UI ที่เป็นหน้าต่างลอย เช่น FileBrowser

private:
    void drawLoadObjectDialog(const std::string& selected_path);

    Editor& m_editor;
    Engine& m_engine;
    IAllocator& m_allocator;

};

} // namespace AEngine