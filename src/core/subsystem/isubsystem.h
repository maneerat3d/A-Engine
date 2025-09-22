#pragma once

#include "core/string/string.h"

namespace AEngine {

class Engine;

class ISubsystem {
public:
    // Constructor จะรับ StringView และใช้ Allocator จาก context (Engine)
    ISubsystem(Engine& context, StringView name);
    virtual ~ISubsystem() = default;

    virtual bool init() = 0;
    virtual void update(float dt) = 0;
    virtual void shutdown() = 0;

    const String& getName() const { return m_name; }

protected:
    Engine& m_context;
    String m_name;
};

} // namespace AEngine