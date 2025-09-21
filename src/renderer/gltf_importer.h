#pragma once
#include <string>
#include <vector>
#include <memory>

namespace AEngine {
class Mesh;
class ResourceManager;

class GltfImporter {
public:
    static std::vector<std::shared_ptr<Mesh>> load(const std::string& path, ResourceManager& resourceManager);
};

} // namespace AEngine