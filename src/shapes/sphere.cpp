#include "sphere.hpp"

Sphere::Sphere(int size) {
    build();
    manageBuffers();
}

void Sphere::build() {
    triangles.clear();
    triangles_colors.clear();

    lines.clear();
    lines_colors.clear();

    std::vector<glm::vec3> v;
    int n = resolution;

    // create first face points
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            v.push_back(glm::vec3((float)i / (n-1) * 2 - 1, (float)j / (n-1) * 2 - 1, 1));
        }
    }

    // normalise point distance
    for (auto &point : v) {
        point = (glm::normalize(point) * normalise_amount + point * (1 - normalise_amount)) / 2.0f;
        point = point * size;
    }

    // first face triangles / mesh
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            triangles.push_back(v[i * n + j]);
            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j]);

            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j]);

            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

            lines.push_back(v[i * n + j]);
            lines.push_back(v[i * n + j + 1]);

            lines.push_back(v[(i + 1) * n + j]);
            lines.push_back(v[(i + 1) * n + j + 1]);

            lines.push_back(v[i * n + j]);
            lines.push_back(v[(i + 1) * n + j]);

            lines.push_back(v[i * n + j + 1]);
            lines.push_back(v[(i + 1) * n + j + 1]);

            lines.push_back(v[i * n + j + 1]);
            lines.push_back(v[(i + 1) * n + j]);

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

    int size_triangles = triangles.size();
    int size_lines = lines.size();
    glm::quat q;

    // copy and rotate face 5 times
    for (glm::quat q : {
        glm::quat(glm::vec3(0, M_PI_2, 0)),
        glm::quat(glm::vec3(0, M_PI, 0)),
        glm::quat(glm::vec3(0, -M_PI_2, 0)),
        glm::quat(glm::vec3(M_PI_2, 0, 0)),
        glm::quat(glm::vec3(-M_PI_2, 0, 0))
    }) {
        for (int i = 0; i < size_triangles; i++) {
            triangles.push_back(triangles[i] * q);
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
        }

        for (int i = 0; i < size_lines; i++) {
            lines.push_back(lines[i] * q);
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }

}

void Sphere::draw() {

    if (rebuild) {
        build();
        manageBuffers();
    }

    if (draw_faces) {
        // TRIANGLES
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, triangles_color_buffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, triangles.size());

        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if (draw_mesh) {
        // LINES
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glDrawArrays(GL_LINES, 0, lines.size());
    }


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

void Sphere::debug() {
    if (ImGui::CollapsingHeader("Sphere")) {
        ImGui::Checkbox("Build each frame", &rebuild);
        ImGui::SliderInt("Resolution:", &resolution, 2, 100);
        ImGui::SliderFloat("Radius:", &size, 0, 100);
        ImGui::Checkbox("Draw mesh:", &draw_mesh);
        ImGui::Checkbox("Draw faces:", &draw_faces);
        ImGui::SliderFloat("Normalisation amound:", &normalise_amount, 0, 1);
    }
}