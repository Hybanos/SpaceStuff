#include "render/objects/orbits.hpp"

namespace render::orbits {

void init() {
    for (int i = 0; i < 360; i++) {
        float i_rad = glm::radians((float) 360);
        vertices[i] = glm::vec3(glm::cos(i_rad), 0, -glm::sin(i_rad));
        colors[i] = glm::vec4(i_rad);
    }
}

void draw(ECSTable &ecs, size_t first, size_t n) {
    // TODO: fix ugly
    if (vertices[0].z != 1) {
        init();
    }

    mesh.set_buffer(0, vertices.data(), vertices.size());
    mesh.set_buffer(1, colors.data(), colors.size());
    mesh.set_buffer(2, ecs.component_table[ROTATION], n, 1);
    mesh.set_buffer(5, ecs.component_table[ORBIT], n, 1);

    
}

} // namespace render::orbits
