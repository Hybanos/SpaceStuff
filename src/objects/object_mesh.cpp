#include "objects/object_mesh.hpp"
#include "scene/scene.hpp"

void ObjectMesh::manage_m_buffers() {
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * 3 * sizeof(float), (float *)lines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines_colors.size() * 4 * sizeof(float), (float *)lines_colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ObjectMesh::draw_m() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, lines.size());
    glBindVertexArray(0);

    scene->lines_drawn += lines.size() / 2;
}