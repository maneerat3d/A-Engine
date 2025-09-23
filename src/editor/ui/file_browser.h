#pragma once
#include "core/string/string.h"
#include <string>
#include <vector>
#include <functional>

namespace AEngine {

class FileBrowser {
public:
    FileBrowser(IAllocator& allocator);
    ~FileBrowser();

    // เปิด File Browser
    // title: ชื่อหน้าต่าง
    // default_path: พาธเริ่มต้น
    // extensions: รายชื่อนามสกุลไฟล์ที่อนุญาต (เช่น {"glb", "gltf"})
    // on_file_selected: Callback ที่จะถูกเรียกเมื่อผู้ใช้เลือกไฟล์
    void open(const char* title, const char* default_path, const std::vector<std::string>& extensions, std::function<void(const std::string&)> on_file_selected);

    // วาด UI ของ File Browser
    void onGUI();

    bool isOpen() const { return m_is_open; }

private:
    IAllocator& m_allocator;
    String m_current_path;
    String m_selected_file;
    std::vector<std::string> m_allowed_extensions;
    std::function<void(const std::string&)> m_on_file_selected;
    bool m_is_open = false;
    char m_filename_buffer[256]; // สำหรับ InputText
    String m_window_title;
};

} // namespace AEngine