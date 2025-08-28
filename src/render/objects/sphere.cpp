#include "render/objects/sphere.hpp"

namespace render::sphere {

void init() {
    mesh = new Mesh(Shader("src/shaders/texture.vs", "src/shaders/texture.fs"));

    std::vector<glm::vec3> v;
    int n = 25;

    // create first face points
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            glm::vec3 point((float)i / (n-1) * 2 - 1, (float)j / (n-1) * 2 - 1, 1);
            v.push_back(point);
        }
    }

    // normalise point distance
    for (auto &point : v) {
        point = glm::normalize(point);
    }

    // first face triangles / mesh
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            vertices.push_back(v[i * n + j]);
            vertices.push_back(v[i * n + j + 1]);
            vertices.push_back(v[(i + 1) * n + j]);

            vertices.push_back(v[i * n + j + 1]);
            vertices.push_back(v[(i + 1) * n + j + 1]);
            vertices.push_back(v[(i + 1) * n + j]);
        }
    }

    int vertex_count = vertices.size();
    // copy and rotate face 5 times
    for (glm::quat q : {
        glm::quat(glm::vec3(0, M_PI_2, 0)),
        glm::quat(glm::vec3(0, M_PI, 0)),
        glm::quat(glm::vec3(0, -M_PI_2, 0)),
        glm::quat(glm::vec3(M_PI_2, 0, 0)),
        glm::quat(glm::vec3(-M_PI_2, 0, 0))
    }) {
        for (int i = 0; i < vertex_count; i++) {
            vertices.push_back(vertices[i] * q);
        }
    }
}

void draw(Scene *scene, ECSTable &ecs) {
    if (vertices.size() == 0) {
        init();
    }

    for (size_t i = 0; i < ecs.size(); i++) {
        if ((ecs.bits[i].to_ulong() & DRAWABLE_SPHERE) != DRAWABLE_SPHERE) continue;

        Position &position = ecs.get_Position(i);
        Rotation &rotation = ecs.get_Rotation(i);
        Scale &scale = ecs.get_Scale(i);
        MajorBody &mb = ecs.get_MajorBody(i);
        Ephemeris &ephemeris = ecs.get_Ephemeris(i);
        RotationInfo &rotation_info = ecs.get_RotationInfo(i);

        int tex;
        if (!id_to_texture.contains(mb.major_body_id)) {
            tex = mesh->gen_cubemap(fmt::format("assets/textures/cubemaps/{}", mb.name));
            id_to_texture.emplace(mb.major_body_id, tex);
        }

        tex = id_to_texture.at(mb.major_body_id);
        
        mesh->set_location(0, vertices.data(), vertices.size());

        mesh->set_mat4("MVP", scene->mvp);
        mesh->set_mat3("rota", rotation);
        mesh->set_int("flip", 0);
        mesh->set_float("scale", scale);
        mesh->set_vec3("pos", position - scene->camera->get_center());

        mesh->draw_cubemap(GL_TRIANGLES, 0, vertices.size() * 3, tex);
    }
}
} // namespace render::sphere