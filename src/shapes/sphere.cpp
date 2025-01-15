#include "sphere.hpp"

Sphere::Sphere(int size) {
    std::vector<glm::vec3> v;
    int n = 3;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            v.push_back(glm::vec3(i, j, 1));
        }
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            triangles.push_back(v[i * n + j]);
            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j]);

            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j + 1]);
            triangles.push_back(v[(i + 1) * n]);

            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

            lines.push_back(v[i * n + j]);
            lines.push_back(v[i * n + j + 1]);

            lines.push_back(v[i * n + j + 1]);
            lines.push_back(v[(i + 1) * n + j]);

            lines.push_back(v[(i + 1) * n + j]);
            lines.push_back(v[i * n + j]);

            lines.push_back(v[i * n + j]);
            lines.push_back(v[(i + 1) * n + j]);

            lines.push_back(v[(i + 1) * n + j]);
            lines.push_back(v[(i + 1) * n]);
            
            lines.push_back(v[(i + 1) * n]);
            lines.push_back(v[i * n + j]);

            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }

    manageBuffers();
}

void Sphere::draw() {
    // TRIANGLES
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, triangles.size());

    // LINES
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glDrawArrays(GL_LINES, 0, lines.size());

    // EXIT
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Sphere::manageBuffers() {
    // TRIANGLES
    // buffer
    glGenBuffers(1, &triangles_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * 3 * sizeof(float), (float *)triangles.data(), GL_STATIC_DRAW);

    // color
    glGenBuffers(1, &triangles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, triangles_colors.size() * 3 * sizeof(float), (float *)triangles_colors.data(), GL_STATIC_DRAW);


    // LINES
    // buffer
    glGenBuffers(1, &lines_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * 3 * sizeof(float), (float *)lines.data(), GL_STATIC_DRAW);

    // color
    glGenBuffers(1, &lines_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines_colors.size() * 3 * sizeof(float), (float *)lines_colors.data(), GL_STATIC_DRAW);
}
