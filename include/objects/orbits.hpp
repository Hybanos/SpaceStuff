#ifndef SPACESTUFF_ORBITS_HPP
#define SPACESTUFF_ORBITS_HPP

#include "object_mesh.hpp"
#include "particle.hpp"
#include "tle.hpp"
#include "scene/camera_anchor.hpp"

#include <glm/gtx/polar_coordinates.hpp>
#include "fmt/core.h"

#include <array>
#include <chrono>
#include <cstdint>

class Orbits : virtual public Object, virtual public CameraAnchor {
    private:
        std::vector<TLE> tle;
        std::vector<glm::mat3> base;
        std::vector<float> semi_major_axis;
        std::vector<float> semi_minor_axis;
        // std::vector<float> linear_eccentricity;
        // std::vector<float> real_time_mean_anomaly;
        std::vector<double> epoch;
        std::vector<float> true_anomaly;
        std::vector<std::array<float, 360>> true_anomaly_index;
        std::vector<glm::vec3> offset;
        std::vector<glm::vec3> pos;
        std::vector<glm::vec2> angle;
        // TODO: switch flags to a proper bitmap
        std::vector<float> flag;

        GLuint matrix_id;
        GLuint base_buffer;
        GLuint offset_buffer;
        GLuint major_buffer;
        GLuint minor_buffer;
        GLuint anomaly_buffer;
        GLuint flag_buffer;

        GLuint VAO;
        GLuint lines_buffer;
        GLuint lines_color_buffer;
        std::vector<glm::vec3> lines;
        std::vector<glm::vec4> lines_colors;

        size_t ttr;
        ImGuiTextFilter filter;
        // index of the satelite to anchor camera on
        int following = 0; 

        void get_true_anomaly(int i, bool recompute);
        void compute_true_anomalies(int i);
        void compute_along_orbit(int i);

        void manage_buffers();
    public:
        Orbits(Scene *, std::vector<TLE>& t, Object *p = nullptr);

        void build_orbit(int i);
        void build();
        void draw();
        void debug();
        glm::vec3 &get_camera_center();
};

#endif