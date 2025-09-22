#pragma once
#include "world_manager.h"
#include "world.h"
#include <iostream>

namespace AEngine {

WorldManager::WorldManager(Engine& context)
    : ISubsystem(context, "WorldManager"), m_active_world(nullptr)
{
}

WorldManager::~WorldManager()
{
}

bool WorldManager::init() {
    std::cout << "WorldManager is initializing..." << std::endl;
    m_active_world = std::make_unique<World>();
    
    // ตั้งค่า Camera เริ่มต้น (ย้ายมาจาก Engine::init() เดิม)
    m_active_world->getCamera().setPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    m_active_world->getCamera().setPosition({0.0f, 2.0f, 5.0f});
    m_active_world->getCamera().lookAt({0.0f, 0.0f, 0.0f});
    
    return true;
}

void WorldManager::update(float dt) {
    // ปัจจุบัน WorldManager ยังไม่มี Logic ที่ต้องทำในแต่ละเฟรม
}

void WorldManager::shutdown() {
    std::cout << "WorldManager is shutting down." << std::endl;
    m_active_world.reset(); // ทำลาย World ที่มีอยู่
}

} // namespace AEngine