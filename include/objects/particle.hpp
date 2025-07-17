#ifndef SPACESTUFF_PARTICLE_HPP
#define SPACESTUFF_PARTICLE_HPP

#include "objects/object.hpp"
#include "render/mesh.hpp"

class Particle : virtual public Object {
    private:
        std::vector<glm::vec3> triangles;
        std::vector<glm::vec4> triangles_colors;

        Mesh mesh;

        bool d_draw = true;

    public:
        glm::vec3 pos;
        glm::vec4 color;

        Particle(Scene *s, glm::vec3 p, glm::vec4 c); 

        void build();
        void draw();
        void debug();
};

#endif