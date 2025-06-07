#include "objects/particle.hpp"
#include "scene/scene.hpp"

Particle::Particle(Scene *s, glm::vec3 p, glm::vec4 c) : Object(s), pos(p), color(c) {
    draw_faces = true;
    draw_mesh = false;
}

void Particle::build() {
    // always face camera
    glm::vec3 dir = glm::normalize(scene->camera->get_position() - pos);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 right = glm::normalize(glm::cross(up, dir));
    glm::vec3 cam_up = glm::cross(dir, right);

    // constant-ish size
    float scale = glm::length(scene->camera->get_position()) * tan(glm::radians(80.0f) / 2) * 2 * 5 / scene->get_width();

    int slices = 25;
    std::vector<glm::vec3> v;
    triangles.clear();
    triangles_colors.clear();

    for (int i = 0; i < slices + 1; i++) {
        float angle = ((float) i / slices) * M_PI * 2; 

        float x = cos(angle) * scale;
        float y = sin(angle) * scale;

        v.push_back(pos + right * x + cam_up * y);
    }

    for (int i = 0; i < slices; i++) {
        triangles.push_back(pos);
        triangles.push_back(v[i]);
        triangles.push_back(v[i+1]);

        triangles_colors.push_back(color);
        triangles_colors.push_back(color);
        triangles_colors.push_back(color);
    }

    manage_f_buffers();
    build_default_mesh();
    manage_m_buffers();
}

void Particle::draw() {
    build();

    glUseProgram(scene->base_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &(scene->mvp)[0][0]);


    if (draw_mesh) {
        draw_m();
    }
    if (draw_faces) {
        draw_f();
    }
}

void Particle::debug() {
    if (ImGui::CollapsingHeader("Particle")) {
        ImGui::Checkbox("Draw mesh:", &draw_mesh);
        ImGui::Checkbox("Draw faces:", &draw_faces);
        ImGui::Text("x: %f, y:%f, z:%f", pos.x, pos.y, pos.z);
    }

}