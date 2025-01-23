#include "grid.hpp"
#include "scene/scene.hpp"

Grid::Grid(Scene * s) : Object{s} {
    draw_faces = true;

    triangles = {
        glm::vec3(-1, 0, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(-1, 0, -1),

        glm::vec3(1, 0, 1),
        glm::vec3(1, 0, -1),
        glm::vec3(-1, 0, -1),
    };

    triangles_colors = {
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),

        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
    };

    manage_f_buffers();
}

void Grid::draw() {
    glUseProgram(scene->base_program_id);

    draw_f();
}