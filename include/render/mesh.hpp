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

        std::vector<GLuint> buffers;
        GLuint texture;
    public:
        Mesh(Shader &s);

        void draw(GLenum type, int first, size_t count);
        void draw_instanced(GLenum type, int first, size_t count, size_t total);
        void draw_cubemap(GLenum type, int first, size_t count);

        template<typename T> 
        void set_buffer(int loc, std::vector<T, std::allocator<T>> &data, int div = 0);

        void gen_cubemap(std::string path);

        void set_mat4(std::string name, glm::mat4 data) {shader.set_mat4(name, data);}
        void set_mat3(std::string name, glm::mat3 data) {shader.set_mat3(name, data);}
        void set_vec3(std::string name, glm::vec3 data) {shader.set_vec3(name, data);}
        void set_int(std::string name, int data) {shader.set_int(name, data);}
};

template<typename T>
void Mesh::set_buffer(int loc, std::vector<T, std::allocator<T>> &data, int div) {
    glBindVertexArray(VAO);

    GLuint buf = buffers[shader.loc_to_buff(loc)];
    int element_count = shader.loc_to_type_size(loc);
    int element_type = shader.loc_to_type(loc);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);

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