#include "sphere.hpp"
#include "scene/scene.hpp"

Sphere::Sphere(Scene *s, std::string files[6]) : ObjectCubeMap(files), Object(s) {
    draw_faces = false;
    draw_mesh = false;

    build();
    manageBuffers();
    manage_texture();
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
            triangles_colors.push_back(triangles_colors[i]);
        }

        for (int i = 0; i < size_lines; i++) {
            lines.push_back(lines[i] * q);
            lines_colors.push_back(lines_colors[i]);
        }
    }
}

void Sphere::draw() {
    if (rebuild) {
        build();
        manageBuffers();
    }

    glm::mat4 mvp = scene->mvp;

    glUseProgram(scene->base_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
    if (draw_faces) draw_f();
    if (draw_mesh) draw_m();
    glUseProgram(scene->texture_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
    if (draw_texture) draw_t();
}

void Sphere::manageBuffers() {
    manage_f_buffers();
    manage_m_buffers();
}

void Sphere::debug() {
    if (ImGui::CollapsingHeader("Sphere")) {
        ImGui::Checkbox("Build each frame", &rebuild);
        ImGui::SliderInt("Resolution:", &resolution, 2, 100);
        ImGui::SliderFloat("Radius:", &size, 0, 100);
        ImGui::Checkbox("Draw mesh:", &draw_mesh);
        ImGui::Checkbox("Draw faces:", &draw_faces);
        ImGui::Checkbox("Draw texture:", &draw_texture);
        ImGui::SliderFloat("Normalisation amound:", &normalise_amount, 0, 1);
    }
}