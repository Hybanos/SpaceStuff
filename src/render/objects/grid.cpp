#include "render/objects/grid.hpp"

namespace render::grid {

void init() {
    mesh = new Mesh(Shader("src/shaders/grid.vs", "src/shaders/grid.fs"));

    float scale = 10010;
    vertices = {
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

    mesh->set_location(0, vertices.data(), vertices.size());
} 

void draw(Scene *scene) {

    if (vertices.size() == 0) {
        init();
    }

    mesh->set_mat4("MVP", scene->mvp);
    mesh->set_vec3("pos", glm::vec3(0));

    mesh->draw(GL_TRIANGLES, 0, vertices.size() * 3);
    scene->triangles_drawn += vertices.size() / 3;
}

} // namespace render::grid