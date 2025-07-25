#pragma once

#include "objects/object.hpp"
#include "render/mesh.hpp"

class Sphere : virtual public Object, virtual public CameraAnchor {
    private:
        std::vector<glm::vec3> triangles;
        std::vector<glm::vec3> triangles_colors;

        Mesh mesh;
        
        bool d_draw = true;
        bool rebuild = false;
        int resolution = 25;
        float size = 6371;
        glm::mat3 rota = glm::mat3(1);
        float normalise_amount = 1.0f;
        void manage_buffers();
    public:
        Sphere(Scene *s, std::string path);
        Sphere(Scene *s, int _id);
        void draw();
        void build();
        void debug();
        glm::vec3 get_camera_center();
};