#include "objects/object_cubemap.hpp"
#include "scene/scene.hpp"

ObjectCubeMap::ObjectCubeMap(std::string _f[6]) {
    for (int i = 0; i < 6; i++) files.push_back(_f[i]);

    glGenTextures(1, &texture_id);
    cubemap_id = glGetUniformLocation(scene->texture_program_id, "cubemap");
    matrix_id = glGetUniformLocation(scene->texture_program_id, "MVP");
}

void ObjectCubeMap::manage_texture() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height, nb_channels;

    for (int i = 0; i < 6; i++) {
        unsigned char *data = nullptr;
        data = stbi_load(files[i].c_str(), &width, &height, &nb_channels, 0);

        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "error loading texture " << files[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void ObjectCubeMap::draw_t() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    glUniform1i(cubemap_id, 0);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisableVertexAttribArray(0);

    scene->triangles_t_drawn += triangles.size() / 3;
}