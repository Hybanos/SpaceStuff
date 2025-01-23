#ifndef SPACESTUFF_ORBIT_LINE_HPP
#define SPACESTUFF_ORBIT_LINE_HPP

#include "object_mesh.hpp"
#include "data/tle.hpp"

#include "glm/gtx/polar_coordinates.hpp"

class OrbitLine : public ObjectMesh {
    private:
        TLE tle;
        glm::mat3 base;
        float semi_major_axis;
        float semi_minor_axis;
        float linear_eccentricity;
        float true_anomaly;
        glm::vec3 offset;

        GLuint matrix_id;
        GLuint base_id;
        GLuint offset_id;
        GLuint a_id;
        GLuint b_id;
        void compute_true_anomaly();
    public:
        OrbitLine(Scene *, TLE t);

        void build_orbit();
        void build();
        void draw();
        void debug();
};

#endif