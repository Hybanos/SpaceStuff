#ifndef SPACESTUFF_OBJECT_FACES_HPP
#define SPACESTUFF_OBJECT_FACES_HPP

#include "object.hpp"

class ObjectFaces : virtual public Object {
    protected:
        GLuint triangles_buffer;
        GLuint triangles_color_buffer;
        std::vector<glm::vec3> triangles;
        std::vector<glm::vec4> triangles_colors;

        bool draw_faces = true;
        void manage_f_buffers();
    public:
        ObjectFaces() {
            glGenBuffers(1, &triangles_buffer);
            glGenBuffers(1, &triangles_color_buffer);
        }

        virtual void draw_f();
};

#endif