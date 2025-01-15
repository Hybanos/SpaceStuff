#include "shapes/object.hpp"

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GL/gl.h"

#include "glm/glm.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

class Sphere : public Object {
    private:
        static const int verticesCount = 363;
        GLuint colorBuffer;
        GLuint vertexBuffer;
        GLfloat colors[verticesCount];
        // GLfloat vertices[verticesCount];
        void manageBuffers();
        std::vector<glm::vec3> vertices;
    public:
        Sphere(int);
        void draw();
};