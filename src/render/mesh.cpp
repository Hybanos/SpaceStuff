#include "render/mesh.hpp"

Mesh::Mesh(Shader &s) : shader{s} {
    glGenVertexArrays(1, &VAO);
    buffers.resize(shader.buff_count());
    std::cout << buffers.size() << std::endl;
    glGenBuffers(buffers.size(), buffers.data());
}

void Mesh::draw() {
    shader.use();

}

void Mesh::draw_instanced(GLenum type, int first, size_t count, size_t total) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawArraysInstanced(type, first, count, total);
    glBindVertexArray(0);
}
