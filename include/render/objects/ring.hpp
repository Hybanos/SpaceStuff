#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ecs/ecs.hpp"
#include "render/mesh.hpp"
#include "render/shader.hpp"
#include "scene/scene.hpp"

namespace render::ring {
    static Mesh *mesh = nullptr;

    static std::vector<glm::vec3> vertices;

    void init();
    void draw(Scene *scene, ECSTable &ecs, size_t first, size_t n);
}