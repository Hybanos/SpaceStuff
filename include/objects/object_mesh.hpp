#ifndef SPACESTUFF_OBJECT_MESH_HPP
#define SPACESTUFF_OBJECT_MESH_HPP

#include "object.hpp"

class ObjectMesh : virtual public Object {
    protected:
        GLuint lines_buffer;
        GLuint lines_color_buffer;
        std::vector<glm::vec3> lines;
        std::vector<glm::vec4> lines_colors;

        bool draw_mesh = true;
        void manage_m_buffers();
    public:
        ObjectMesh() {
            glGenBuffers(1, &lines_buffer);
            glGenBuffers(1, &lines_color_buffer);
        }

        virtual void draw_m();
};

#endif