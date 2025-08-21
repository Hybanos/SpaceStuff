#pragma once

#include <vector>

#include "ecs/ecs.hpp"
#include "render/mesh.hpp"
#include "render/shader.hpp"
#include "scene/scene.hpp"

namespace render::grid {
    static Mesh *mesh = nullptr;

    static std::vector<glm::vec3> vertices;

    void init();
    void draw(Scene *scene);

} // namespace render:grid: