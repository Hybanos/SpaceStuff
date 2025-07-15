#include "render/mesh.hpp"

Mesh::Mesh(Shader &s) : shader{s} {
    glGenVertexArrays(1, &VAO);
    buffers.resize(shader.buff_count());
    glGenBuffers(buffers.size(), buffers.data());
}

void Mesh::draw() {

}

template<class T>
void Mesh::set_buffer(int loc, std::vector<T> &data, int div) {
    glBindVertexArray(VAO);

    GLuint buf = buffers[shader.loc_to_buff(loc)];
    int element_count = shader.loc_to_type_size(loc);
    int element_type = shader.loc_to_type(loc);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);

    if (element_count <= 4) {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, element_count, element_type, GL_TRUE, 0, (void *) 0);
        glVertexAttribDivisor(loc, div);
    } else {

        int locs_used, elements_per_loc;

        if (element_count % 3 == 0) {
            locs_used = 3;
            elements_per_loc = element_count / 3;
        } else {
            // TODO: make sure this is correct for non-square matrices
            // (maybe dividing by 4 isn't correct because of some col/row major shit)
            locs_used = 4;
            elements_per_loc = element_count / 4;
        }

        for (int i = 0; i < locs_used; i++) {
            glEnableVertexAttribArray(loc + i);
            glVertexAttribPointer(loc + i, elements_per_loc, element_type, GL_TRUE, sizeof(T) , (void *) (i * sizeof(T) / locs_used));
            glVertexAttribDivisor(loc + i, div);
        }
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// template<class T>
// void Mesh::set_buffer(int loc, std::vector<T> &data) {
//     set_buffer(loc, data, 0);
// }