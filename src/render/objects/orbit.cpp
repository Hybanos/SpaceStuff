#include "render/objects/orbit.hpp"

namespace render::orbit {

void init() {
    mesh = new Mesh(Shader("src/shaders/orbits.vs", "src/shaders/orbits.fs"));
    mesh->fuse_loc_buffers(5, 9);
    vertices.resize(360);
    colors.resize(360);
    for (int i = 0; i < 360; i++) {
        float i_rad = glm::radians((float) i);
        vertices[i] = glm::vec3(glm::cos(i_rad), 0, -glm::sin(i_rad));
        colors[i] = glm::vec4(i_rad);
    }
    vertices.push_back(vertices[0]);
    colors.push_back(colors[0]);
}

void draw(Scene *scene, ECSTable &ecs) {
    if (vertices.size() == 0) init();
    
    for (auto &a : ecs.arch_iter(DRAWABLE_ORBIT, false)) {
        mesh->set_location(0, vertices.data(), vertices.size());
        mesh->set_location(1, colors.data(), colors.size());
        mesh->set_location(2, &a->get_Rotation(0), a->size(), 1);
        mesh->fill_buffer(5, &a->get_Orbit(0), a->size());
        size_t offset = 0;
        mesh->set_attrib_pointer(5, 3, sizeof(Orbit), (void *) offset);
        mesh->set_attrib_divisor(5, 1);
        offset += sizeof(glm::vec3);
        mesh->set_attrib_pointer(6, 1, sizeof(Orbit), (void *) offset);
        mesh->set_attrib_divisor(6, 1);
        offset += sizeof(float);
        mesh->set_attrib_pointer(7, 1, sizeof(Orbit), (void *) offset);
        mesh->set_attrib_divisor(7, 1);
        offset += sizeof(float);
        mesh->set_attrib_pointer(8, 1, sizeof(Orbit), (void *) offset);
        mesh->set_attrib_divisor(8, 1);
        offset += sizeof(float);
        mesh->set_attrib_pointer(9, 1, sizeof(Orbit), (void *) offset);
        mesh->set_attrib_divisor(9, 1);

        mesh->set_mat4("MVP", scene->mvp);
        mesh->set_vec3("pos", glm::vec3(0) - scene->camera->get_center());

        mesh->draw_instanced(GL_LINE_STRIP, 0, vertices.size(), a->size());
        scene->lines_drawn += vertices.size() * a->size();
    }
}

} // namespace render::orbit
