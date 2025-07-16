#ifndef SPACESTUFF_SPHERE_HPP
#define SPACESTUFF_SPHERE_HPP

// #include "object_mesh.hpp"
// #include "object_faces.hpp"
// #include "object_cubemap.hpp"
#include "objects/object.hpp"
#include "render/mesh.hpp"

class Sphere : virtual public Object{
    private:
        std::vector<glm::vec3> triangles;
        std::vector<glm::vec3> triangles_colors;
        std::vector<glm::vec3> lines;
        std::vector<glm::vec3> lines_colors;

        Mesh mesh;
        
        bool rebuild = false;
        int resolution = 25;
        float size = 6371;
        glm::mat3 rota = glm::mat3(1);
        float normalise_amount = 1.0f;
        void manage_buffers();
    public:
        Sphere(Scene *, std::string[6], Object *p = nullptr);
        void draw();
        void build();
        void debug();
        glm::vec3 get_pos();
};

#endif