#ifndef SPACESTUFF_OBJECT_MESH_HPP
#define SPACESTUFF_OBJECT_MESH_HPP

#include "object.hpp"

class ObjectMesh : virtual public Object {
    protected:
        GLuint lines_buffer;
        GLuint lines_color_buffer;
        std::vector<glm::vec3> lines;
        std::vector<glm::vec3> lines_colors;

        bool draw_mesh = true;
    public:
        virtual void draw_m() = 0;
};

#endif