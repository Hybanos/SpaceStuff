#pragma once

#define SECS_DAY (24 * 3600)
#define SECS_SIDERAL_DAY (23 * 3600 + 56 * 60 + 4)
#define LAST_VERNAL_EQUINOX 1742424540

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "scene/signal.hpp"
#include "scene/camera_anchor.hpp"

class Scene;

class Object {
    protected:
        Scene *scene = nullptr;
        glm::vec3 pos = glm::vec3(0);
        std::vector<Object *> children;

        // horizons system body id;
        int id = 0;
        std::string display_name;
    public:

        Object(Scene *s) : scene{s} {}

        virtual void draw() = 0;
        virtual void debug() = 0;

        virtual void on_signal(Signal s) {} 
        Object *add_child(Object * c) {children.push_back(c); return this;}
        void set_pos(glm::vec3 p) {pos = p;}
};
