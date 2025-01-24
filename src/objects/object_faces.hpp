#ifndef SPACESTUFF_OBJECT_FACES_HPP
#define SPACESTUFF_OBJECT_FACES_HPP

#include "object_mesh.hpp"

class ObjectFaces : virtual public ObjectMesh {
    protected:
        GLuint triangles_buffer;
        GLuint triangles_color_buffer;
        std::vector<glm::vec3> triangles;
        std::vector<glm::vec4> triangles_colors;

        bool draw_faces = true;
        void build_default_mesh();
        void manage_f_buffers();
    public:
        ObjectFaces();

        virtual void draw_f();
};

#endif