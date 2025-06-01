#ifndef SPACESTUFF_GRID_HPP
#define SPACESTUFF_GRID_HPP

#include "object_faces.hpp"

class Grid : public ObjectFaces {
    private:
        GLuint matrix_id;

        float scale = 10010;
    public:
        Grid(Scene *);
        void draw();
        void debug();
};

#endif