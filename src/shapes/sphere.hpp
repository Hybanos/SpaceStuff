#include "shapes/object.hpp"

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GL/gl.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

class Sphere : public Object {
    private:
        GLuint triangles_buffer;
        GLuint triangles_color_buffer;
        std::vector<glm::vec3> triangles;
        std::vector<glm::vec3> triangles_colors;

        GLuint lines_buffer;
        GLuint lines_color_buffer;
        std::vector<glm::vec3> lines;
        std::vector<glm::vec3> lines_colors;

        bool rebuild = false;
        bool draw_mesh = true;
        bool draw_faces = true;
        int resolution = 4;
        float size = 1;
        float normalise_amount = 0.0f;

        void manageBuffers();
    public:
        Sphere(int);
        void draw();
        void build();
        void debug();
};