#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "resource.h"
#include "plugins/gltf_importer/gltf_importer.h" 
// Forward declare
namespace AEngine {


class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager();

    void registerImporter(const std::vector<std::string>& extensions, AEngine::IResourceImporter* importer);

    template <typename T, typename... Args>
    std::shared_ptr<T> load(const std::string& path, Args&&... args) {
        // 1. ตรวจสอบว่าเคยโหลด Resource นี้แล้วหรือยัง (เหมือนเดิม)        
        if (m_resources.find(path) != m_resources.end()) {
            return std::static_pointer_cast<T>(m_resources[path]);
        }

        // 2. ตรวจสอบว่าเป็นการ "สร้าง" หรือ "โหลด"
        if constexpr (sizeof...(args) > 0) {
            // 2.1 ถ้ามี arguments เพิ่มเติม (เช่น vertices, indices) ให้ถือว่าเป็นการ "สร้าง" โดยตรง
            auto resource = std::make_shared<T>(*this, path, std::forward<Args>(args)...);
            m_resources[path] = resource;
            return resource;
        } else {
            // 2.2 ถ้าไม่มี arguments เพิ่มเติม ให้ถือว่าเป็นการ "โหลด" จากไฟล์
            size_t dot_pos = path.rfind('.');
            if (dot_pos == std::string::npos) {
                // ไม่มีนามสกุลไฟล์, ไม่รู้จะโหลดยังไง
                return nullptr;
            }
            std::string ext = path.substr(dot_pos);

            if (m_importers.count(ext)) {
                // **หัวใจของระบบ**: ถ้าเจอนามสกุลไฟล์ที่ลงทะเบียนไว้
                IResourceImporter* importer = m_importers.at(ext);
                if (importer->load(path, *this)) {
                    // สั่งให้ importer โหลดไฟล์ ถ้าสำเร็จ...
                    // ถ้า Importer โหลดสำเร็จ มันควรจะสร้าง Resource ไว้ใน map แล้ว เราแค่ดึงมันออกมา
                    return std::static_pointer_cast<T>(m_resources[path]);
                }
            }
        }
        return nullptr; // ถ้าไม่เจอ Importer หรือโหลดไม่สำเร็จ
    }

    // เพิ่มฟังก์ชันนี้เข้ามา
    template <typename T>
    std::vector<std::shared_ptr<T>> findAll(const std::string& basePath) const {
        std::vector<std::shared_ptr<T>> foundResources;
        for (const auto& pair : m_resources) {
            const std::string& path = pair.first;
            
            // 1. ตรวจสอบว่า path ของ resource "ขึ้นต้นด้วย" basePath ที่ระบุหรือไม่
            if (path.rfind(basePath, 0) == 0) { // rfind(str, 0) == 0 คือการเช็ค "startsWith"
                 
                // 2. ตรวจสอบ Type ของ resource โดยใช้ dynamic_pointer_cast
                auto specificResource = std::dynamic_pointer_cast<T>(pair.second);
                if (specificResource) {
                    // 3. ถ้าแปลง Type สำเร็จ (แสดงว่าเป็น Type ที่ถูกต้อง) ก็เพิ่มลงใน vector ผลลัพธ์
                    foundResources.push_back(specificResource);
                }
            }
        }
        return foundResources;
    }
    

private:
    std::unordered_map<std::string, std::shared_ptr<AEngine::Resource>> m_resources; // <--- เพิ่ม AEngine::

    std::unordered_map<std::string, AEngine::IResourceImporter*> m_importers;
};

} // namespace AEngine