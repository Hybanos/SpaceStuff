#pragma once

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ecs/ecs.hpp"
#include "render/mesh.hpp"
#include "render/shader.hpp"
#include "scene/scene.hpp"

namespace render::sphere {
    static Mesh *mesh = nullptr;

    static std::vector<glm::vec3> vertices;
    static std::unordered_map<int, int> id_to_texture;

    void init();
    void draw(Scene *scene, ECSTable &ecs);
}