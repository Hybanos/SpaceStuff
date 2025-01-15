#include "sphere.hpp"

Sphere::Sphere(int size) {
    std::vector<glm::vec3> v;
    int n = 10;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            v.push_back(glm::vec3(i, j, 1));
        }
    }

    for (int i = 0; i < (n - 1); i++) {
        for (int j = 0; j < (n - 1); j++) {
            vertices.push_back(v[i * n + j]);
            vertices.push_back(v[i * n + j + 1]);
            vertices.push_back(v[(i + 1) * n + j]);

            vertices.push_back(v[i * n + j]);
            vertices.push_back(v[(i + 1) * n + j]);
            vertices.push_back(v[(i + 1) * n]);
        }
    }

    manageBuffers();
}

void Sphere::draw() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Sphere::manageBuffers() {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), (float *)vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}
