#include "sphere.hpp"

Sphere::Sphere(int size) {
    program_id = LoadShaders("src/shaders/texture.vs", "src/shaders/texture.fs");

    glGenBuffers(1, &triangles_buffer);
    glGenBuffers(1, &triangles_color_buffer);
    glGenBuffers(1, &lines_buffer);
    glGenBuffers(1, &lines_color_buffer);

    build();
    manage_texture();
    manageBuffers();
}

void Sphere::build() {
    triangles.clear();
    triangles_colors.clear();

    lines.clear();
    lines_colors.clear();

    std::vector<glm::vec3> v;
    int n = resolution;

    // create first face points
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            v.push_back(glm::vec3((float)i / (n-1) * 2 - 1, (float)j / (n-1) * 2 - 1, 1));
        }
    }

    // normalise point distance
    for (auto &point : v) {
        point = (glm::normalize(point) * normalise_amount + point * (1 - normalise_amount)) / 2.0f;
        point = point * size;
    }

    // first face triangles / mesh
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            triangles.push_back(v[i * n + j]);
            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j]);

            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j]);

            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

            lines.push_back(v[i * n + j]);
            lines.push_back(v[i * n + j + 1]);

            lines.push_back(v[(i + 1) * n + j]);
            lines.push_back(v[(i + 1) * n + j + 1]);

            lines.push_back(v[i * n + j]);
            lines.push_back(v[(i + 1) * n + j]);

            lines.push_back(v[i * n + j + 1]);
            lines.push_back(v[(i + 1) * n + j + 1]);

            lines.push_back(v[i * n + j + 1]);
            lines.push_back(v[(i + 1) * n + j]);

            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            lines_colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }

    int size_triangles = triangles.size();
    int size_lines = lines.size();
    glm::quat q;

    // copy and rotate face 5 times
    for (glm::quat q : {
        glm::quat(glm::vec3(0, M_PI_2, 0)),
        glm::quat(glm::vec3(0, M_PI, 0)),
        glm::quat(glm::vec3(0, -M_PI_2, 0)),
        glm::quat(glm::vec3(M_PI_2, 0, 0)),
        glm::quat(glm::vec3(-M_PI_2, 0, 0))
    }) {
        for (int i = 0; i < size_triangles; i++) {
            triangles.push_back(triangles[i] * q);
            triangles_colors.push_back(triangles_colors[i]);
        }

        for (int i = 0; i < size_lines; i++) {
            lines.push_back(lines[i] * q);
            lines_colors.push_back(lines_colors[i]);
        }
    }
}

void Sphere::draw(glm::mat4 mvp) {
    if (rebuild) {
        build();
        manageBuffers();
    }

    if (draw_faces) draw_f();
    if (draw_mesh) draw_m();
    if (draw_texture) draw_t(mvp);
}

void Sphere::draw_f() {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, triangles.size());

    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Sphere::draw_m() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_LINES, 0, lines.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Sphere::draw_t(glm::mat4 mvp) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    glUseProgram(program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
    glUniform1i(cubemap_id, 0);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(vao);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisableVertexAttribArray(0);
}

void Sphere::manageBuffers() {
    // TRIANGLES
    // buffer
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * 3 * sizeof(float), (float *)triangles.data(), GL_STATIC_DRAW);

    // color
    glBindBuffer(GL_ARRAY_BUFFER, triangles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, triangles_colors.size() * 3 * sizeof(float), (float *)triangles_colors.data(), GL_STATIC_DRAW);

    // LINES
    // buffer
    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * 3 * sizeof(float), (float *)lines.data(), GL_STATIC_DRAW);

    // color
    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines_colors.size() * 3 * sizeof(float), (float *)lines_colors.data(), GL_STATIC_DRAW);

    // TEXTURE
    cubemap_id = glGetUniformLocation(program_id, "cubemap");
    matrix_id = glGetUniformLocation(program_id, "MVP");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * 3 * sizeof(float), (float *)triangles.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

void Sphere::manage_texture() {
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height, nb_channels;
    std::vector<std::string> faces = {
        "assets/cubemaps/earth/front.jpg",
        "assets/cubemaps/earth/back.jpg",
        "assets/cubemaps/earth/top.jpg",
        "assets/cubemaps/earth/bottom.jpg",
        "assets/cubemaps/earth/left.jpg",
        "assets/cubemaps/earth/right.jpg",
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

}

void Sphere::debug() {
    if (ImGui::CollapsingHeader("Sphere")) {
        ImGui::Checkbox("Build each frame", &rebuild);
        ImGui::SliderInt("Resolution:", &resolution, 2, 100);
        ImGui::SliderFloat("Radius:", &size, 0, 100);
        ImGui::Checkbox("Draw mesh:", &draw_mesh);
        ImGui::Checkbox("Draw faces:", &draw_faces);
        ImGui::Checkbox("Draw texture:", &draw_texture);
        ImGui::SliderFloat("Normalisation amound:", &normalise_amount, 0, 1);
    }
}