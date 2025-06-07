#ifndef SPACESTUFF_ORBIT_LINE_HPP
#define SPACESTUFF_ORBIT_LINE_HPP

#include "object_mesh.hpp"
#include "particle.hpp"
#include "tle.hpp"

#include <glm/gtx/polar_coordinates.hpp>

class OrbitLine : public ObjectMesh {
    private:
        TLE tle;
        glm::mat3 base;
        float semi_major_axis;
        float semi_minor_axis;
        float linear_eccentricity;
        double epoch;
        float real_time_mean_anomaly;
        float true_anomaly;
        glm::vec3 offset;
        glm::vec3 pos;
        glm::vec2 angle;

        GLuint matrix_id;
        GLuint base_id;
        GLuint offset_id;
        GLuint a_id;
        GLuint b_id;

        bool show_apsis = false;
        Particle periapsis;
        Particle apoapsis;

        void compute_true_anomaly();
        void compute_pitch_yaw();
    public:
        OrbitLine(Scene *, TLE t);

        void build_orbit();
        void build();
        void draw();
        void debug();
};

#endif