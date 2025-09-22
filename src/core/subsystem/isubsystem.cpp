#include "isubsystem.h"
#include "engine/engine.h" // ต้อง include engine.h เพื่อใช้ getAllocator

namespace AEngine {

ISubsystem::ISubsystem(Engine& context, StringView name)
    : m_context(context)
    , m_name(name, context.getAllocator()) // สร้าง String ด้วย Allocator ของ Engine
{}

} // namespace AEngine