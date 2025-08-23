#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <cctype>

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/gl.h"
#include "fmt/core.h"

#include "render/shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "render/stb_image.h"

class Mesh {
    private:
        GLuint VAO;
        Shader shader;

        std::vector<int> loc_to_buffer_override;

        std::vector<GLuint> buffers;
        std::vector<GLuint> texture;
    public:
        Mesh(Shader s);

        void draw(GLenum type, int first, size_t count, int tex = 0);
        void draw_instanced(GLenum type, int first, size_t count, size_t total, int tex = 0);
        void draw_cubemap(GLenum type, int first, size_t count, int tex = 0);

        template<typename T> 
        void set_location(int loc, T *data, size_t count, int div = 0);
        template<typename T> 
        void fill_buffer(int loc, T *data, size_t count);
        void set_attrib_pointer(int loc, int element_count, int stride, void *pointer);
        void set_attrib_divisor(int loc, int div);
        // TODO: that's ugly
        void fuse_loc_buffers(int first, int last);

        int gen_cubemap(std::string path);
        int gen_texture(std::string path);

        void set_mat4(std::string name, glm::mat4 data) {shader.set_mat4(name, data);}
        void set_mat3(std::string name, glm::mat3 data) {shader.set_mat3(name, data);}
        void set_vec3(std::string name, glm::vec3 data) {shader.set_vec3(name, data);}
        void set_vec2(std::string name, glm::vec2 data) {shader.set_vec2(name, data);}
        void set_int(std::string name, int data) {shader.set_int(name, data);}
        void set_float(std::string name, float data) {shader.set_float(name, data);}
        void set_bitset(std::string name, int *data, int count) {shader.set_bitset(name, data, count);}
};

template<typename T>
void Mesh::set_location(int loc, T *data, size_t count, int div) {
    glBindVertexArray(VAO);

    GLuint buf = buffers[shader.loc_to_buff(loc)];
    int element_count = shader.loc_to_type_size(loc);
    int element_type = shader.loc_to_type(loc);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), data, GL_STATIC_DRAW);

    // fmt::print("buff id: {}, loc: {}, count: {}, type: {}\n",shader.loc_to_buff(loc), loc, element_count, element_type);

    if (element_count <= 4) {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, element_count, element_type, GL_FALSE, 0, (void *) 0);
        glVertexAttribDivisor(loc, div);
    } else {

        int locs_used, elements_per_loc;

        if ((element_count % 3) == 0) {
            locs_used = 3;
            elements_per_loc = element_count / 3;
        } else {
            // TODO: make sure this is correct for non-square matrices
            // (maybe dividing by 4 isn't correct because of some col/row major shit)
            locs_used = 4;
            elements_per_loc = element_count / 4;
        }

        // fmt::print("locs used: {}, elements per: {}\n", locs_used, elements_per_loc);

        for (int i = 0; i < locs_used; i++) {
            glEnableVertexAttribArray(loc + i);
            glVertexAttribPointer(loc + i, elements_per_loc, element_type, GL_FALSE, sizeof(T), (void *) (i * (sizeof(T) / locs_used)));
            glVertexAttribDivisor(loc + i, div);
        }
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T> 
void Mesh::fill_buffer(int loc, T *data, size_t count) {
    glBindVertexArray(VAO);

    GLuint buf = buffers[shader.loc_to_buff(loc)];
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * count, data, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}