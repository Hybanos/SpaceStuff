#pragma once

#include <nlohmann/json.hpp>

#include "objects/object.hpp"
#include "render/mesh.hpp"
#include "data/position.hpp"
#include "objects/particle.hpp"

using json = nlohmann::json;

class Ring : virtual public Object {
    private:
        std::vector<glm::vec3> triangles;
        std::vector<float> transmittance;
        std::vector<float> radius;
        glm::vec2 highlight_bounds = glm::vec2(0);

        std::string name;
        int selected = -1;

        Mesh mesh;
        json ring_info;

        void build();
    public:
        Ring(Scene *s, std::string body_name);

        void draw();
        void debug();
};