#ifndef SPACESTUFF_CUBE_HPP
#define SPACESTUFF_CUBE_HPP

#include "shapes/object.hpp"

#include <iostream>

#include "GL/glew.h"
#include "GL/gl.h"

#include "glm/glm.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

class Cube : public Object {
    private:
        static const int verticesCount = 9*12;
        GLuint colorBuffer;
        GLuint vertexBuffer;
        GLfloat colors[verticesCount];
        GLfloat vertices[verticesCount] = {
            // Front
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f,  1.0f, 1.0f,
            
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,

            // Back
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
            
            0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Top
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,

            // Bottom
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,

            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f,

            // Right
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,

            // Left
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,

        };
        void manageBuffers();
    public:
        Cube(int);
        Cube(int, float, float, float);
        ~Cube();
        void draw();
        void debug() {}
        void move(glm::vec3);
};

#endif