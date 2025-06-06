#include "objects/object_faces.hpp"
#include "scene/scene.hpp"

ObjectFaces::ObjectFaces() {
    glGenBuffers(1, &triangles_buffer);
    glGenBuffers(1, &triangles_color_buffer);
}

void ObjectFaces::build_default_mesh() {

    lines.clear();

    for (int i = 0; i < triangles.size(); i += 3) {
        lines.push_back(triangles[i]);
        lines.push_back(triangles[i+1]);

        lines.push_back(triangles[i+1]);
        lines.push_back(triangles[i+2]);
    
        lines.push_back(triangles[i+2]);
        lines.push_back(triangles[i]);

        lines_colors.push_back(glm::vec4(0,0,0,1));
        lines_colors.push_back(glm::vec4(0,0,0,1));
        lines_colors.push_back(glm::vec4(0,0,0,1));
        lines_colors.push_back(glm::vec4(0,0,0,1));
        lines_colors.push_back(glm::vec4(0,0,0,1));
        lines_colors.push_back(glm::vec4(0,0,0,1));
    }
}

void ObjectFaces::manage_f_buffers() {
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * 3 * sizeof(float), (float *)triangles.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, triangles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, triangles_colors.size() * 4 * sizeof(float), (float *)triangles_colors.data(), GL_STATIC_DRAW);
}

void ObjectFaces::draw_f() {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_color_buffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, triangles.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_POLYGON_OFFSET_FILL);

    scene->triangles_drawn += triangles.size() / 3;
}