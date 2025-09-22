#pragma once

// -- PHASE 1: Decoupling the Core Engine --
// Step 1.1: Create Subsystem Context
// ISubsystem คือ Interface กลางสำหรับระบบย่อยทั้งหมดใน Engine
// ทำหน้าที่กำหนดวงจรชีวิต (Lifecycle) พื้นฐานคือ init, update, และ shutdown

#include <string>

namespace AEngine {

class Engine;

class ISubsystem {
public:
    ISubsystem(Engine& context, const std::string& name) : m_context(context), m_name(name) {}
    virtual ~ISubsystem() = default;

    virtual bool init() = 0;
    virtual void update(float dt) = 0;
    virtual void shutdown() = 0;

    const std::string& getName() const { return m_name; }

protected:
    Engine& m_context;
    std::string m_name;
};

} // namespace AEngine