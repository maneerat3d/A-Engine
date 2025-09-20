#include "resource_manager.h"

namespace AEngine {

ResourceManager::~ResourceManager() {
    m_resources.clear();
}

} // namespace AEngine