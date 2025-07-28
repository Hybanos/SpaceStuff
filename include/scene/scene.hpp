#ifndef SPACESTUFF_SCENE_HPP
#define SPACESTUFF_SCENE_HPP

#include "scene/time.hpp"
#include "scene/camera.hpp"
#include "objects/sphere.hpp"
#include "objects/skybox.hpp"
#include "objects/grid.hpp"
#include "objects/orbits.hpp"
#include "objects/barycenter.hpp"
#include "data/db_manager.hpp"
#include "render/shader.hpp"

#include "data/tle.hpp"

#include <vector>
#include <chrono>

class Camera;
class Object;

class Scene {
    private:
        std::vector<Object *> objects;
        Time time;
    public:
        Scene(SDL_Window *);
        DBManager db;

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

        Shader orbits_shader = Shader("src/shaders/orbits.vs", "src/shaders/orbits.fs");
        Shader base_shader = Shader("src/shaders/base.vs", "src/shaders/base.fs");
        Shader texture_shader = Shader("src/shaders/texture.vs", "src/shaders/texture.fs");
        Shader grid_shader = Shader("src/shaders/grid.vs", "src/shaders/grid.fs");
        GLuint base_program_id;

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

#endif