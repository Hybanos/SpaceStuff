#include "objects/skybox.hpp"
#include "scene/scene.hpp"

std::string f[6] = {
    "assets/textures/cubemaps/skybox/px.png",
    "assets/textures/cubemaps/skybox/nx.png",
    "assets/textures/cubemaps/skybox/py.png",
    "assets/textures/cubemaps/skybox/ny.png",
    "assets/textures/cubemaps/skybox/pz.png",
    "assets/textures/cubemaps/skybox/nz.png",
};

SkyBox::SkyBox(Scene *s) : Object(s), mesh(scene->texture_shader) {

    mesh.gen_cubemap("assets/textures/cubemaps/skybox");

    triangles = {
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),

        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),

        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),

        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),

        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),

        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f,  1.0f)
    };

    for (auto &v : triangles) v = v * 100.0f;

    rota = glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1)
    );

    mesh.set_location(0, triangles.data(), triangles.size());
}

void SkyBox::draw() {
    glm::mat4 mvp = scene->projection * glm::mat4(glm::mat3(scene->view)) * scene->model;

    if (d_draw) {
        glDepthMask(GL_FALSE);
        mesh.set_mat4("MVP", mvp);
        mesh.set_mat4("transform", glm::mat4(1));
        mesh.set_mat3("rota", rota);
        mesh.set_int("flip", 1);
        mesh.set_vec3("pos", glm::vec3(0));

        mesh.draw_cubemap(GL_TRIANGLES, 0, triangles.size() * 3);
        glDepthMask(GL_TRUE);
    }
}

void SkyBox::debug() {
    if (ImGui::TreeNode("SkyBox")) {
        ImGui::Checkbox("Draw", &d_draw);
        ImGui::TreePop();
    }
}