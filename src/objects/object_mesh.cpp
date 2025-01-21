#include "object_mesh.hpp"
#include "scene/scene.hpp"

void ObjectMesh::manage_m_buffers() {
    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * 3 * sizeof(float), (float *)lines.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines_colors.size() * 3 * sizeof(float), (float *)lines_colors.data(), GL_STATIC_DRAW);
}

void ObjectMesh::draw_m() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_LINES, 0, lines.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    scene->lines_drawn += lines.size() / 2;
}