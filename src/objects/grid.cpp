#include "objects/grid.hpp"
#include "scene/scene.hpp"

Grid::Grid(Scene * s) : Object{s}, mesh(scene->grid_shader) {
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

    mesh.set_buffer(0, triangles);
}

void Grid::draw() {

    if (d_draw) {
        mesh.set_mat4("MVP", scene->mvp);

        glm::vec3 p = pos;
        if (!follow) p = p - scene->camera->get_center();

        mesh.set_vec3("pos", p);
        mesh.draw(GL_TRIANGLES, 0, triangles.size() * 3);
    }
}

void Grid::debug() {
    if (ImGui::TreeNode("Grid")) {
        ImGui::Checkbox("Draw", &d_draw);
        ImGui::Checkbox("Follow", &follow);
        ImGui::TreePop();
    }

}