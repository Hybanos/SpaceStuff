#include "skybox.hpp"
#include "scene/scene.hpp"

std::string f[6] = {
    "assets/cubemaps/skybox/px.png",
    "assets/cubemaps/skybox/nx.png",
    "assets/cubemaps/skybox/py.png",
    "assets/cubemaps/skybox/ny.png",
    "assets/cubemaps/skybox/pz.png",
    "assets/cubemaps/skybox/nz.png",
};

SkyBox::SkyBox(Scene *s) : ObjectCubeMap(f), Object(s) {
    draw_faces = false;

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

    for (int i = 0; i < triangles.size(); i++) triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

    manage_texture();
    manage_f_buffers();
}

void SkyBox::draw() {
    glm::mat4 mvp = scene->projection * glm::mat4(glm::mat3(scene->view)) * scene->model;

    glDepthMask(GL_FALSE);
    if (draw_faces) {
        glUseProgram(scene->base_program_id);
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
        draw_f();
    }
    if (draw_texture) {
        glUseProgram(scene->texture_program_id);
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
        draw_t();
    }
    glDepthMask(GL_TRUE);
}

void SkyBox::debug() {
    if (ImGui::CollapsingHeader("SkyBox")) {
        ImGui::Checkbox("Draw faces:", &draw_faces);
        ImGui::Checkbox("Draw texture:", &draw_texture);
    }
}