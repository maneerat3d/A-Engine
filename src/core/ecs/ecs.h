#pragma once
#include <cstdint>

namespace AEngine {

using Entity = uint32_t;
const Entity INVALID_ENTITY = 0;

using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 32;

} // namespace AEngine