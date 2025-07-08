#ifndef SPACESTUFF_ORBITS_HPP
#define SPACESTUFF_ORBITS_HPP

#include "object_mesh.hpp"
#include "particle.hpp"
#include "tle.hpp"

#include <glm/gtx/polar_coordinates.hpp>
#include <array>

class Orbits : public ObjectMesh {
    private:
        std::vector<TLE> tle;
        std::vector<glm::mat3> base;
        std::vector<float> semi_major_axis;
        std::vector<float> semi_minor_axis;
        std::vector<float> linear_eccentricity;
        std::vector<double> epoch;
        std::vector<float> real_time_mean_anomaly;
        std::vector<float> true_anomaly;
        std::vector<std::array<float, 360>> true_anomaly_index;
        std::vector<glm::vec3> offset;
        std::vector<glm::vec3> pos;
        std::vector<glm::vec2> angle;

        GLuint matrix_id;
        GLuint base_id;
        GLuint offset_id;
        GLuint a_id;
        GLuint b_id;

        // std::vector<bool> show_apsis;

        void get_true_anomaly(int i, bool recompute);
        void compute_true_anomalies(int i);
        void compute_pitch_yaw(int i);
    public:
        Orbits(Scene *, std::vector<TLE>& t);

        void build_orbit(int i);
        void build(int i);
        void draw();
        void debug();
};

#endif