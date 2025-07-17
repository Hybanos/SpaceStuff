#ifndef SPACESTUFF_GRID_HPP
#define SPACESTUFF_GRID_HPP

#include "objects/object.hpp"
#include "render/mesh.hpp"

class Grid : virtual public Object {
    private:
        std::vector<glm::vec3> triangles;

        Mesh mesh;

        bool d_draw = true;
        float scale = 10010;
    public:
        Grid(Scene *);
        void draw();
        void debug();
};

#endif