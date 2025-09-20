#pragma once

#include "transform.h"
#include <string>
#include <memory>

// Forward declarations
namespace AEngine {
class Mesh;
class Texture;
}

namespace AEngine {

struct GameObject {
    Transform transform;
    std::string name;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
};

} // namespace AEngine