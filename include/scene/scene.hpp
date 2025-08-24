#pragma once

#include "scene/time.hpp"
#include "scene/camera.hpp"
#include "data/db_manager.hpp"
#include "render/shader.hpp"
#include "ecs/ecs.hpp"
#include "data/tle.hpp"

#include <vector>
#include <chrono>

class Camera;

class Scene {
    private:
        ECSTable ecs;

    public:
        Scene(SDL_Window *);
        DBManager db;
        Time time;

        SDL_Window *window = nullptr;
        Camera *camera = nullptr;

        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
        glm::mat4 mvp;
        
        size_t lines_drawn = 0;
        size_t triangles_drawn = 0;
        size_t triangles_t_drawn = 0;
        size_t frames = 0;
        size_t follow_entity = -1;

        size_t ttr;
        bool imgui_hover;

        std::chrono::high_resolution_clock::time_point frame_time;

        float get_ratio();
        int get_width();
        int get_height();
        std::chrono::high_resolution_clock::time_point get_time();

        void build_solar_system();

        void render();
        void debug();
};