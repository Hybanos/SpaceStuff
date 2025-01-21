#ifndef SPACESTUFF_ORBIT_LINE_HPP
#define SPACESTUFF_ORBIT_LINE_HPP

#include "object_mesh.hpp"
#include "data/tle.hpp"

#include "glm/gtx/polar_coordinates.hpp"

class OrbitLine : public ObjectMesh {
    private:
        TLE tle;
    public:
        OrbitLine(Scene *, TLE t);

        void draw();
        void build();
        void debug();
};

#endif