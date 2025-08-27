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

void draw(Scene *scene, ECSTable &ecs, size_t first, size_t n) {
    if (vertices.size() == 0) {
        init();
    }

    for (size_t i = 0; i < ecs.size(); i++) {
        if (ecs.bits[i] != DRAWABLE_RING) continue;

        DisplayName &name = ecs.get_DisplayName(i);
        Parent &parent = ecs.get_Parent(i);
        Ring &ring = ecs.get_Ring(i);

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

} // namespace render::ring