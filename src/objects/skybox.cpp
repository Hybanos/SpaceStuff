#include "objects/skybox.hpp"
#include "scene/scene.hpp"

std::string f[6] = {
    "assets/cubemaps/skybox/px.png",
    "assets/cubemaps/skybox/nx.png",
    "assets/cubemaps/skybox/py.png",
    "assets/cubemaps/skybox/ny.png",
    "assets/cubemaps/skybox/pz.png",
    "assets/cubemaps/skybox/nz.png",
};

SkyBox::SkyBox(Scene *s) : Object(s), mesh(scene->texture_shader) {

    mesh.gen_cubemap(f);

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

    rota = glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1)
    );

    mesh.set_buffer(0, triangles);
}

void SkyBox::draw() {
    glm::mat4 mvp = scene->projection * glm::mat4(glm::mat3(scene->view)) * scene->model;

    if (d_draw) {
        glDepthMask(GL_FALSE);
        mesh.set_mat4("MVP", mvp);
        mesh.set_mat3("rota", rota);
        mesh.set_int("flip", 1);
        mesh.draw_cubemap(GL_TRIANGLES, 0, triangles.size() * 3);
        glDepthMask(GL_TRUE);
    }
}

void SkyBox::debug() {
    if (ImGui::CollapsingHeader("SkyBox")) {
        ImGui::Checkbox("Draw", &d_draw);
    }
}