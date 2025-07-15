#ifndef SPACESTUFF_OBJECT_HPP
#define SPACESTUFF_OBJECT_HPP

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

class Scene;

class Object {
    protected:
        Scene *scene = nullptr;
        Object *parent = nullptr;
    public:

        Object(Scene *s, Object *p = nullptr) : scene{s}, parent{p} {}

        virtual void draw() = 0;
        virtual void debug() = 0;
        virtual glm::vec3 get_pos() {
            if (parent != nullptr) {
                return parent->get_pos();
            }
            return glm::vec3(0);
        }
};

#endif