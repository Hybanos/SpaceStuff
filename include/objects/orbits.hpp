#ifndef SPACESTUFF_ORBITS_HPP
#define SPACESTUFF_ORBITS_HPP

#include <array>
#include <chrono>
#include <cstdint>

#include "glm/gtx/polar_coordinates.hpp"
#include "fmt/core.h"

#include "particle.hpp"
#include "data/tle.hpp"
#include "scene/camera_anchor.hpp"
#include "render/mesh.hpp"

class Orbits : virtual public Object, virtual public CameraAnchor {
    private:
        std::vector<TLE> tle;
        std::vector<glm::mat3> base;
        std::vector<float> semi_major_axis;
        std::vector<float> semi_minor_axis;
        std::vector<double> epoch;
        std::vector<float> true_anomaly;
        std::vector<std::array<float, 360>> true_anomaly_index;
        std::vector<glm::vec3> offset;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> angle;
        // TODO: switch flags to a proper bitmap
        std::vector<float> flag;

        Mesh mesh;
        std::vector<glm::vec3> lines;
        std::vector<glm::vec4> lines_colors;

        size_t ttr;
        ImGuiTextFilter filter;
        bool d_draw = true;
        // index of the satelite to anchor camera on
        int following = 0; 

        void get_true_anomaly(int i, bool recompute);
        void compute_true_anomalies(int i);
        void compute_along_orbit(int i);

        void manage_buffers();
    public:
        Orbits(Scene *, std::vector<TLE>& t);

        void build_orbit(int i);
        void build();

        void on_signal(Signal s);

        void draw();
        void debug();
        glm::vec3 get_camera_center();
};

#endif