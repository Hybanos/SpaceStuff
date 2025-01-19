#include "skybox.hpp"
#include "scene/scene.hpp"

SkyBox::SkyBox() {
    manage_texture();
}

void SkyBox::manage_texture() {
    program_id = LoadShaders("src/shaders/texture.vs", "src/shaders/texture.fs");

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height, nb_channels;
    std::vector<std::string> faces = {
        "assets/cubemaps/skybox/px.png",
        "assets/cubemaps/skybox/nx.png",
        "assets/cubemaps/skybox/py.png",
        "assets/cubemaps/skybox/ny.png",
        "assets/cubemaps/skybox/pz.png",
        "assets/cubemaps/skybox/nz.png",
    };

    for (int i = 0; i < 6; i++) {
        unsigned char *data = nullptr;
        data = stbi_load(faces[i].c_str(), &width, &height, &nb_channels, 0);

        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "error loading texture " << faces[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cubemap_id = glGetUniformLocation(program_id, "cubemap");
    matrix_id = glGetUniformLocation(program_id, "MVP");

    std::cout << vbo << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

void SkyBox::draw(glm::mat4 _) {

    glm::mat4 mvp = scene->projection * glm::mat4(glm::mat3(scene->view)) * scene->model;

    if (draw_texture) draw_t(mvp);
}

void SkyBox::draw_t(glm::mat4 mvp) {
    glDepthMask(GL_FALSE);

    glUseProgram(program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
    glUniform1i(cubemap_id, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisableVertexAttribArray(0);
    glDepthMask(GL_TRUE);
}

void SkyBox::debug() {
    if (ImGui::CollapsingHeader("SkyBox")) {
        ImGui::Checkbox("Draw texture:", &draw_texture);
    }
}