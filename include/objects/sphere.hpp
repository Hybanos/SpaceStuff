#ifndef SPACESTUFF_SPHERE_HPP
#define SPACESTUFF_SPHERE_HPP

#include "object_mesh.hpp"
#include "object_faces.hpp"
#include "object_cubemap.hpp"

class Sphere : public ObjectCubeMap {
    private:
        GLuint cubemap_id;
        bool rebuild = false;
        int resolution = 25;
        float size = 6371;
        glm::mat3 rota = glm::mat3(1);
        float normalise_amount = 1.0f;
        void manageBuffers();
    public:
        Sphere(Scene *, std::string[6]);
        void draw();
        void build();
        void debug();
};

#endif