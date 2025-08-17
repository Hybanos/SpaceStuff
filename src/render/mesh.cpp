#include "render/mesh.hpp"

Mesh::Mesh(Shader s) : shader{s} {
    glGenVertexArrays(1, &VAO);
    buffers.resize(shader.buff_count());
    glGenBuffers(buffers.size(), buffers.data());
}

void Mesh::draw(GLenum type, int first, size_t count) {
    shader.use();

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(type, first, count);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Mesh::draw_cubemap(GLenum type, int first, size_t count) {
    shader.use();

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(type, first, count);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glBindVertexArray(0);
}

void Mesh::draw_instanced(GLenum type, int first, size_t count, size_t total) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawArraysInstanced(type, first, count, total);
    glBindVertexArray(0);
}

void Mesh::gen_texture(std::string path) {
    std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c){return std::tolower(c);}); // help
    glGenTextures(1, &texture);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nb_channels;

    unsigned char *data = nullptr;
    data = stbi_load(path.c_str(), &width, &height, &nb_channels, 0);

    if (data != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    } else {
        std::cout << "error loading texture " << path << std::endl;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    shader.use(); 
    set_int("tex", 0);
}

void Mesh::gen_cubemap(std::string path) {
    std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c){return std::tolower(c);}); // help
    if (! std::filesystem::exists("./" + path)) {
        std::cout << "could not find " << path << std::endl;
        path = "assets/textures/cubemaps/earth";
    }

    glGenTextures(1, &texture);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nb_channels;

    int i = 0;
    for (std::string s : {
        "px.png",
        "nx.png",
        "py.png",
        "ny.png",
        "pz.png",
        "nz.png"
    }) {
        std::string filepath = path + "/" + s;
        unsigned char *data = nullptr;
        data = stbi_load(filepath.c_str(), &width, &height, &nb_channels, 0);

        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "error loading texture " << filepath << std::endl;
        }
        i++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}