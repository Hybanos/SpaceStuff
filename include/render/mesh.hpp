#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include <render/shader.hpp>

class Mesh {
    private:
        GLuint VAO;
        std::vector<GLuint> buffers;
        Shader shader;
    public:
        Mesh(Shader &s);
        void draw();
        // template<class T> 
        // void set_buffer(int loc, std::vector<T> &data);
        template<class T> 
        void set_buffer(int loc, std::vector<T> &data, int div = 0);
};