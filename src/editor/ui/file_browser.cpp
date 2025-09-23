#include "file_browser.h"
#include "core/os.h" // สำหรับ os::FileIterator, os::FileInfo
#include "core/string/string.h"
#include <imgui.h>
#include <algorithm> // สำหรับ std::sort

namespace AEngine {

FileBrowser::FileBrowser(IAllocator& allocator)
    : m_allocator(allocator)
    , m_current_path(allocator)
    , m_selected_file(allocator)
    , m_window_title(allocator)
{
    m_filename_buffer[0] = '\0';
}

FileBrowser::~FileBrowser() {
}

void FileBrowser::open(const char* title, const char* default_path, const std::vector<std::string>& extensions, std::function<void(const std::string&)> on_file_selected) {
    m_window_title = String(title, m_allocator);
    m_current_path = String(default_path, m_allocator);
    m_allowed_extensions = extensions;
    m_on_file_selected = on_file_selected;
    m_selected_file = String("", m_allocator);
    m_filename_buffer[0] = '\0';
    m_is_open = true;
    ImGui::OpenPopup(m_window_title.c_str());
}

void FileBrowser::onGUI() {
    if (!m_is_open) return;

    // ตั้งค่าขนาดและตำแหน่งเริ่มต้นของหน้าต่าง
    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(m_window_title.c_str(), &m_is_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        // แสดง Current Path
        ImGui::Text("Current Path: %s", m_current_path.c_str());
        ImGui::Separator();

        // ปุ่ม Go Up
        if (ImGui::Button("..")) {
            const char* last_slash = AEngine::reverseFind(m_current_path, '/');
            if (last_slash) {
                m_current_path.resize(u32(last_slash - m_current_path.c_str()));
            } else {
                m_current_path = String("", m_allocator); // ไปที่ Root
            }
            if (m_current_path.empty()) m_current_path = String(".", m_allocator); // ถ้าเป็น Root ให้แสดงเป็น "."
            m_selected_file = String("", m_allocator);
            m_filename_buffer[0] = '\0';
        }
        ImGui::SameLine();

        // แสดงรายการไฟล์และโฟลเดอร์
        ImGui::BeginChild("##file_list", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), true);

        std::vector<std::string> directories;
        std::vector<std::string> files;

        // ใช้ os::FileIterator เพื่อวนลูปหาไฟล์และโฟลเดอร์
        os::FileIterator* it = os::createFileIterator(m_current_path.c_str(), m_allocator);
        os::FileInfo info;
        while (os::getNextFile(it, &info)) {
            if (info.filename[0] == '.') continue; // ซ่อนไฟล์/โฟลเดอร์ที่ขึ้นต้นด้วยจุด

            if (info.is_directory) {
                directories.push_back(info.filename);
            } else {
                // ตรวจสอบนามสกุลไฟล์
                size_t dot_pos = std::string(info.filename).rfind('.');
                if (dot_pos != std::string::npos) {
                    std::string ext = std::string(info.filename).substr(dot_pos);
                    // แปลงเป็น lowercase เพื่อเปรียบเทียบ
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                    bool allowed = false;
                    if (m_allowed_extensions.empty()) {
                        allowed = true; // ถ้าไม่กำหนดนามสกุล ให้แสดงทั้งหมด
                    } else {
                        for (const auto& allowed_ext : m_allowed_extensions) {
                            if (ext == allowed_ext) {
                                allowed = true;
                                break;
                            }
                        }
                    }
                    if (allowed) {
                        files.push_back(info.filename);
                    }
                }
            }
        }
        os::destroyFileIterator(it);

        // เรียงลำดับ
        std::sort(directories.begin(), directories.end());
        std::sort(files.begin(), files.end());

        // แสดงโฟลเดอร์
        for (const auto& dir : directories) {
            if (ImGui::Selectable((std::string("[DIR] ") + dir).c_str(), false, ImGuiSelectableFlags_DontClosePopups)) {
                m_current_path.add(StringView("/"));
                m_current_path.add(StringView(dir.c_str()));
                m_selected_file = String("", m_allocator);
                m_filename_buffer[0] = '\0';
            }
        }

        // แสดงไฟล์
        for (const auto& file : files) {
            bool is_selected = (m_selected_file == file.c_str());
            if (ImGui::Selectable(file.c_str(), is_selected, ImGuiSelectableFlags_DontClosePopups)) {
                m_selected_file = String(file.c_str(), m_allocator);
                AEngine::copyString(AEngine::Span(m_filename_buffer), m_selected_file);
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                // เลือกไฟล์และปิด
                std::string full_path = std::string(m_current_path.c_str()) + "/" + m_selected_file.c_str();
                m_on_file_selected(full_path);
                m_is_open = false;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndChild();

        // Input สำหรับชื่อไฟล์
        ImGui::InputText("File Name", m_filename_buffer, sizeof(m_filename_buffer));

        // ปุ่ม Open/Cancel
        if (ImGui::Button("Open")) {
            if (!m_selected_file.empty()) {
                std::string full_path = std::string(m_current_path.c_str()) + "/" + m_selected_file.c_str();
                m_on_file_selected(full_path);
                m_is_open = false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_is_open = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

} // namespace AEngine