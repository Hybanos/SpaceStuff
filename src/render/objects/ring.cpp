#include "render/objects/ring.hpp"

namespace render::ring {

void init() {
    mesh = new Mesh(Shader("src/shaders/ring.vs", "src/shaders/ring.fs"));
    mesh->gen_texture("assets/textures/other/perlin-512_6-oct.png");

    vertices = {
        glm::vec3(-1, 0, -1),
        glm::vec3(-1, 0, 1),
        glm::vec3(1, 0, 1),

        glm::vec3(-1, 0, -1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 0, -1),
    };

    mesh->set_location(0, vertices.data(), vertices.size());
}

void draw(Scene *scene, ECSTable &ecs) {
    if (vertices.size() == 0) init();

    for (auto &a : ecs.arch_iter(DRAWABLE_RING, false)) {

        for (size_t i = 0; i < a->size(); i++) {
            DisplayName &name = a->get_DisplayName(i);
            Parent &parent = a->get_Parent(i);
            Ring &ring = a->get_Ring(i);
            Position &pos = ecs.get_Position(parent);
            Rotation &rota = ecs.get_Rotation(parent);

            mesh->set_mat4("MVP", scene->mvp);
            mesh->set_vec3("pos", pos - scene->camera->get_center());
            mesh->set_mat3("rota", rota);
            mesh->set_vec2("range", ring.range);
            mesh->set_float("transmittance", ring.transmittance);
            mesh->set_int("selected", ring.selected);
            mesh->set_bitset("bitset", (int *) &ring.bits, 16);

            mesh->draw(GL_TRIANGLES, 0, vertices.size() * 3);
        }
    }
}

} // namespace render::ring