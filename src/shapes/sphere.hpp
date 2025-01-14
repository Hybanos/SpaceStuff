#define GLM_ENABLE_EXPERIMENTAL
# define PI           3.14159265358979323846

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "SFML/OpenGL.hpp"
#include "../../glm/glm.hpp"
#include "../../glm/gtx/transform.hpp"
#include "../../glm/gtc/matrix_transform.hpp"

class Sphere {
    private:
        static const int verticesCount = 363;
        GLuint colorBuffer;
        GLuint vertexBuffer;
        GLfloat colors[verticesCount];
        // GLfloat vertices[verticesCount];
        void manageBuffers();
        vector<GLfloat> vertices;
    public:
        Sphere(int);
        void draw();
};