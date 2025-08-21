#pragma once

#include "ecs/ecs.hpp"
#include "render/mesh.hpp"
#include "render/shader.hpp"
#include "scene/scene.hpp"

#include <vector>

namespace render::orbit {
    static Mesh *mesh = nullptr;

    static std::vector<glm::vec3> vertices;
    static std::vector<glm::vec4> colors;

    // static Scene *scene;

    void init();
    void draw(Scene *scene, ECSTable &ecs, size_t first, size_t n);
}