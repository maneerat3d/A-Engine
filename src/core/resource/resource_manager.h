#pragma once
// -- PHASE 1: Decoupling the Core Engine --
// Step 1.3: Elevate ResourceManager and World
// ปรับแก้ให้ ResourceManager เป็น Subsystem เต็มตัว
#include "core/subsystem/isubsystem.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "resource.h"
#include "plugins/gltf_importer/gltf_importer.h"
#include "core/container/hash_map.h" // ใช้ HashMap ของเรา
#include "core/string/string.h"      // ใช้ String ของเรา
// Forward declare
namespace AEngine {


class ResourceManager final : public ISubsystem { // <--- เพิ่มการสืบทอด
public:
    ResourceManager(Engine& context);
    ~ResourceManager();

    // --- ISubsystem Implementation ---
    bool init() override;
    void update(float dt) override;
    void shutdown() override;
    // ---------------------------------

    void registerImporter(const std::vector<std::string>& extensions, AEngine::IResourceImporter* importer);

    // --- เพิ่มฟังก์ชันนี้ ---
    // ฟังก์ชันสำหรับให้ Importer เพิ่ม Resource ที่สร้างเสร็จแล้วเข้ามาในระบบ
    void add(const std::string& path, std::shared_ptr<AEngine::Resource> resource) {
        FilePathHash path_hash(path.c_str());
        // ตรวจสอบก่อนว่ามีอยู่แล้วหรือไม่ เพื่อป้องกันการเขียนทับโดยไม่ตั้งใจ
        if (!m_resources.find(path_hash).isValid()) {
            m_resources.insert(path_hash, resource);
        }
    }
    // --- สิ้นสุดส่วนที่เพิ่ม ---

    template <typename T, typename... Args>
    std::shared_ptr<T> load(const std::string& path, Args&&... args) {
        // 1. ตรวจสอบว่าเคยโหลด Resource นี้แล้วหรือยัง
        FilePathHash path_hash(path.c_str());
        auto resource_iter = m_resources.find(path_hash);
        if (resource_iter.isValid()) {
            return std::static_pointer_cast<T>(resource_iter.value());
        }

        // 2. ตรวจสอบว่าเป็นการ "สร้าง" หรือ "โหลด"
        if constexpr (sizeof...(args) > 0) {
            // 2.1 ถ้ามี arguments เพิ่มเติม ให้ถือว่าเป็นการ "สร้าง" โดยตรง
            auto resource = std::make_shared<T>(*this, path, std::forward<Args>(args)...);
            m_resources.insert(path_hash, resource);
            return resource;
        } else {
            // 2.2 ถ้าไม่มี arguments เพิ่มเติม ให้ถือว่าเป็นการ "โหลด" จากไฟล์
            size_t dot_pos = path.rfind('.');
            if (dot_pos == std::string::npos) {
                return nullptr;
            }

            std::string ext_str = path.substr(dot_pos);
            // สร้าง hash จาก extension string
            u32 ext_hash = AEngine::RuntimeHash32(ext_str.c_str()).getHashValue();

            // --- นี่คือส่วนที่แก้ไข ---
            auto iter = m_importers.find(ext_hash); // 1. เรียก find() และเก็บผลลัพธ์
            if (iter.isValid()) {                   // 2. ใช้ isValid() ในการเช็ค
                // **หัวใจของระบบ**: ถ้าเจอนามสกุลไฟล์ที่ลงทะเบียนไว้
                IResourceImporter* importer = iter.value(); // 3. ใช้ value() เพื่อดึงค่า
                if (importer->load(path, *this)) {
                    // สั่งให้ importer โหลดไฟล์ ถ้าสำเร็จ...
                    // ถ้า Importer โหลดสำเร็จ มันควรจะสร้าง Resource ไว้ใน map แล้ว เราแค่ดึงมันออกมา
                    // *** บรรทัดนี้จะทำงานถูกต้องแล้วหลังจากแก้ไข Importer ***
                    auto new_resource_iter = m_resources.find(path_hash);
                    if (new_resource_iter.isValid()) { // <--- เพิ่มการตรวจสอบความปลอดภัย
                        return std::static_pointer_cast<T>(new_resource_iter.value());
                    }
                }
            }
        }
        return nullptr; // ถ้าไม่เจอ Importer หรือโหลดไม่สำเร็จ
    }

    // เพิ่มฟังก์ชันนี้เข้ามา
    template <typename T>
    std::vector<std::shared_ptr<T>> findAll(const std::string& basePath) const {
        std::vector<std::shared_ptr<T>> foundResources;
        // 'resource_ptr' ที่ได้จาก loop คือ std::shared_ptr<AEngine::Resource> โดยตรง
        for (const auto& resource_ptr : m_resources) {
            // ดึง path มาจากตัว resource object
            const std::string& path = resource_ptr->getPath();

            // 1. ตรวจสอบว่า path ของ resource "ขึ้นต้นด้วย" basePath ที่ระบุหรือไม่
            if (path.rfind(basePath, 0) == 0) { // rfind(str, 0) คือการเช็ค "startsWith"

                // 2. ตรวจสอบ Type ของ resource โดยใช้ dynamic_pointer_cast
                auto specificResource = std::dynamic_pointer_cast<T>(resource_ptr);
                if (specificResource) {
                    // 3. ถ้าแปลง Type สำเร็จ (แสดงว่าเป็น Type ที่ถูกต้อง) ก็เพิ่มลงใน vector ผลลัพธ์
                    foundResources.push_back(specificResource);
                }
            }
        }
        return foundResources;
    }
private:
    HashMap<FilePathHash, std::shared_ptr<AEngine::Resource>> m_resources;

    HashMap<u32, AEngine::IResourceImporter*> m_importers;
};

} // namespace AEngine