#ifndef SPACESTUFF_SCENE_HPP
#define SPACESTUFF_SCENE_HPP

#include "scene/camera.hpp"
#include "objects/sphere.hpp"
#include "objects/skybox.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <SDL3/SDL.h>
#include <GL/gl.h>
#include <GL/glew.h>

#include <vector>

class Camera;
class Object;

class Scene {
    private:
        std::vector<Object *> objects;
        size_t frames = 0;
    public:
        Scene(SDL_Window*);

        GLuint program_id;
        GLuint matrix_id;

        SDL_Window *window;
        Camera *camera = nullptr;

        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;

        float get_ratio();
        int get_width();
        int get_height();

        void render();
        void debug();
};

#endif