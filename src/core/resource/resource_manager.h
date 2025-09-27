#pragma once

#include "core/subsystem/isubsystem.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "i_resource_importer.h"

// Forward declarations สำหรับสิ่งที่ใช้เป็นแค่ Pointer/Reference ใน header นี้
namespace AEngine {
    class Engine;
}

namespace AEngine {

class ResourceManager final : public ISubsystem {
public:
    ResourceManager(Engine& context);
    ~ResourceManager();

    // --- ISubsystem Implementation ---
    bool init() override;
    void update(float dt) override;
    void shutdown() override;

    // --- Public API ---
    void registerImporter(const std::vector<std::string>& extensions, IResourceImporter* importer);
    void add(const std::string& path, std::shared_ptr<Resource> resource);

    template <typename T, typename... Args>
    std::shared_ptr<T> load(const std::string& path, Args&&... args);

    template <typename T>
    std::vector<std::shared_ptr<T>> findAll(const std::string& basePath) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
    std::unordered_map<std::string, IResourceImporter*> m_importers;
};

template <typename T, typename... Args>
std::shared_ptr<T> ResourceManager::load(const std::string& path, Args&&... args) {
    // --- ขั้นตอนที่ 1: ตรวจสอบในแคช (Cache) ก่อน ---
    // ค้นหาใน map ว่ามี resource ที่ใช้ path นี้อยู่แล้วหรือไม่
    auto it = m_resources.find(path);
    if (it != m_resources.end()) {
        // ถ้าเจอ, แสดงว่าเคยโหลดไปแล้ว ไม่ต้องทำอะไรเพิ่ม
        // ทำการแปลง (cast) smart pointer ให้เป็นประเภทที่ผู้เรียกต้องการแล้วส่งคืน
        return std::static_pointer_cast<T>(it->second);
    }

    // --- ขั้นตอนที่ 2: แยกแยะระหว่างการ "สร้าง" (Create) และการ "โหลด" (Load) ---
    // ใช้ `if constexpr` เพื่อให้ Compiler ตัดสินใจตั้งแต่ตอนคอมไพล์
    // `sizeof...(args) > 0` หมายถึงมีการส่งพารามิเตอร์เพิ่มเติมเข้ามานอกเหนือจาก path
    if constexpr (sizeof...(args) > 0) {
        // --- กรณี "สร้าง" Resource ใหม่ ---
        // เมื่อมี arguments เพิ่มเติมเข้ามา, เราถือว่าผู้ใช้ต้องการ "สร้าง" object ใหม่โดยตรง
        // เช่น load<Texture>("some_id", width, height, data);
        auto resource = std::make_shared<T>(*this, path, std::forward<Args>(args)...);
        // เพิ่ม resource ที่สร้างใหม่เข้าไปใน map เพื่อให้ครั้งต่อไปหาเจอในแคช
        m_resources.emplace(path, resource);
        return resource;
    }
    // ถ้าไม่มี arguments เพิ่มเติม, เราจะเข้าสู่กระบวนการ "โหลด" จากไฟล์
    else {
        // --- กรณี "โหลด" Resource จากไฟล์ ---
        // ค้นหาตำแหน่งของจุด '.' สุดท้ายใน path เพื่อหานามสกุลไฟล์
        size_t dot_pos = path.rfind('.');
        if (dot_pos == std::string::npos) {
            // ถ้าไม่เจอนามสกุลไฟล์, เราไม่สามารถหา Importer ที่เหมาะสมได้
            return nullptr;
        }

        // ตัดสตริงเพื่อดึงเฉพาะส่วนที่เป็นนามสกุลไฟล์ (เช่น ".glb", ".png")
        std::string extension = path.substr(dot_pos);

        // ค้นหา Importer ที่ลงทะเบียนไว้กับนามสกุลไฟล์นี้
        auto importer_it = m_importers.find(extension);
        if (importer_it != m_importers.end()) {
            // ถ้าเจอ Importer ที่ตรงกัน
            IResourceImporter* importer = importer_it->second;

            // สั่งให้ Importer ตัวนั้นทำงาน โดยส่ง path และตัวจัดการ Resource นี้เข้าไป
            // Importer ที่ดี เมื่อโหลดสำเร็จ จะต้องเรียก `resourceManager.add(...)` ด้วยตัวเอง
            if (importer->load(path, *this)) {
                // หาก Importer ทำงานสำเร็จและคืนค่า true
                // เราต้องค้นหา resource ใน map อีกครั้ง เพราะ Importer ควรจะเพิ่มมันเข้ามาให้แล้ว
                auto new_resource_it = m_resources.find(path);
                if (new_resource_it != m_resources.end()) {
                    // ถ้าเจอ ก็แปลงประเภทแล้วส่งคืนให้ผู้ใช้
                    return std::static_pointer_cast<T>(new_resource_it->second);
                }
            }
        }
    }

    // ถ้าไม่เจอ Importer, โหลดไม่สำเร็จ, หรือเกิดปัญหาอื่นๆ จะคืนค่า nullptr
    return nullptr;
}

template <typename T>
std::vector<std::shared_ptr<T>> ResourceManager::findAll(const std::string& basePath) const {
    // สร้าง vector ว่างๆ เพื่อเก็บผลลัพธ์
    std::vector<std::shared_ptr<T>> foundResources;
    // วนลูปผ่าน resource ทั้งหมดที่เก็บไว้ใน map
    for (const auto& pair : m_resources) {
        // `pair.first` คือ path (string), `pair.second` คือ resource (shared_ptr)
        const std::string& path = pair.first;

        // ตรวจสอบว่า path ของ resource "ขึ้นต้นด้วย" basePath ที่ผู้ใช้ระบุหรือไม่
        // `rfind(basePath, 0) == 0` เป็นวิธีเช็ค "startsWith" ใน C++ ที่มีประสิทธิภาพ
        if (path.rfind(basePath, 0) == 0) {
            // ถ้า path ตรงเงื่อนไข, ต่อไปเราต้องเช็คว่า "ประเภท" ของ resource ตรงกับที่ผู้ใช้ต้องการหรือไม่
            // ใช้ `dynamic_pointer_cast` เพื่อลองแปลงประเภทของ pointer
            auto specificResource = std::dynamic_pointer_cast<T>(pair.second);
            if (specificResource) {
                // ถ้าการแปลงสำเร็จ (ได้ pointer ที่ไม่ใช่ nullptr) แสดงว่าเป็น resource ประเภทเดียวกัน
                // เพิ่ม resource ที่เจอลงใน vector ผลลัพธ์
                foundResources.push_back(specificResource);
            }
        }
    }
    // คืนค่า vector ที่มี resource ทั้งหมดที่ตรงเงื่อนไข
    return foundResources;
}

} // namespace AEngine

// รวมไฟล์ implementation ของ template เข้ามา (ยังคงทำเหมือนเดิม)
