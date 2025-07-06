#ifndef SPACESTUFF_ORBITS_HPP
#define SPACESTUFF_ORBITS_HPP

#include "object_mesh.hpp"
#include "particle.hpp"
#include "tle.hpp"

#include <glm/gtx/polar_coordinates.hpp>

class Orbits : public ObjectMesh {
    private:
        std::vector<TLE> tle;
        glm::mat3 base;
        float semi_major_axis;
        float semi_minor_axis;
        float linear_eccentricity;
        double epoch;
        float real_time_mean_anomaly;
        float true_anomaly;
        float true_anomaly_index[360];
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

        void get_true_anomaly(int i, bool recompute);
        void compute_true_anomalies();
        void compute_pitch_yaw(int i);
    public:
        Orbits(Scene *, std::vector<TLE>& t);

        void build_orbit(int i);
        void build(int i);
        void draw();
        void debug();
};

#endif