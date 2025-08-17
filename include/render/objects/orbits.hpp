#pragma once

#include "ecs/ecs.hpp"
#include "render/mesh.hpp"
#include "render/shader.hpp"

#include <vector>

namespace render::orbits {
    static Mesh mesh(Shader("src/shaders/orbits.vs", "src/shaders/orbits.fs"));

    static std::vector<glm::vec3> vertices;
    static std::vector<glm::vec4> colors;

    void init();
    void draw(ECSTable &ecs, size_t first, size_t n);
}