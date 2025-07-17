#ifndef SPACESTUFF_SKYBOX_HPP
#define SPACESTUFF_SKYBOX_HPP

#include "objects/object.hpp"
#include "render/mesh.hpp"

class SkyBox : virtual public Object {
    private:
        std::vector<glm::vec3> triangles;

        Mesh mesh;

        bool d_draw = true;
        glm::mat3 rota;
    public:
        SkyBox(Scene *);
        void draw();
        void debug();
};

#endif