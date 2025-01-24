#include "grid.hpp"
#include "scene/scene.hpp"

Grid::Grid(Scene * s) : Object{s} {
    draw_faces = true;
    draw_mesh = false;

    matrix_id = glGetUniformLocation(scene->grid_program_id, "MVP");

    triangles = {
        glm::vec3(-1,  0,  1) * scale,
        glm::vec3( 1,  0,  1) * scale,
        glm::vec3(-1,  0, -1) * scale,

        glm::vec3( 1,  0,  1) * scale,
        glm::vec3( 1,  0, -1) * scale,
        glm::vec3(-1,  0, -1) * scale,

        glm::vec3(0,  1, -1) * scale,
        glm::vec3(0,  1,  1) * scale,
        glm::vec3(0, -1, -1) * scale,

        glm::vec3(0,  1,  1) * scale,
        glm::vec3(0, -1,  1) * scale,
        glm::vec3(0, -1, -1) * scale,

        glm::vec3( 1, -1, 0) * scale,
        glm::vec3( 1,  1, 0) * scale,
        glm::vec3(-1, -1, 0) * scale,

        glm::vec3( 1,  1, 0) * scale,
        glm::vec3(-1,  1, 0) * scale,
        glm::vec3(-1, -1, 0) * scale,
    };

    triangles_colors = {
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),

        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),

        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),

        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),

        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),

        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
    };

    manage_f_buffers();
    build_default_mesh();
    manage_m_buffers();
}

void Grid::draw() {
    if (draw_mesh) {
        draw_m();
        glUseProgram(scene->base_program_id);
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &(scene->mvp)[0][0]);
    }

    if (draw_faces) {
        glUseProgram(scene->grid_program_id);
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &(scene->mvp)[0][0]);

        draw_f();
    }
}

void Grid::debug() {
    if (ImGui::CollapsingHeader("Grid")) {
        ImGui::Checkbox("Draw mesh:", &draw_mesh);
        ImGui::Checkbox("Draw faces:", &draw_faces);
    }

}