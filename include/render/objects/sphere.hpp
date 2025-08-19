#pragma once

#include "ecs/ecs.hpp"
#include "render/mesh.hpp"
#include "render/shader.hpp"
#include "scene/scene.hpp"

#include <vector>

namespace render::sphere {
    static Mesh *mesh = nullptr;

    static std::vector<glm::vec3> vertices;

    void init();
    void draw(Scene *scene, ECSTable &ecs, size_t first, size_t n);
}