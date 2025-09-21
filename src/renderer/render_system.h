#pragma once
#include "ecs/isystem.h"

// Forward declarations
struct SDL_Window;

namespace AEngine {
class Renderer;

class RenderSystem : public ISystem {
public:
    RenderSystem(SDL_Window* window);
    ~RenderSystem();

    void init() override;
    void update(Scene& scene, float dt) override;
    void shutdown() override;

private:
    Renderer* m_renderer;
    SDL_Window* m_window;
};

} // namespace AEngine