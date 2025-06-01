#ifndef SPACESTUFF_SCENE_HPP
#define SPACESTUFF_SCENE_HPP

#include "scene/camera.hpp"
#include "objects/sphere.hpp"
#include "objects/skybox.hpp"
#include "objects/grid.hpp"
#include "objects/orbit_line.hpp"

#include "tle.hpp"

#include <vector>

class Camera;
class Object;

class Scene {
    private:
        std::vector<Object *> objects;
    public:
        Scene(SDL_Window *);

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

        GLuint base_program_id;
        GLuint texture_program_id;
        GLuint orbit_program_id;
        GLuint grid_program_id;

        float get_ratio();
        int get_width();
        int get_height();

        void render();
        void debug();
};

#endif