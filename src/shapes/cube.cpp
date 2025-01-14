#include "shapes/cube.hpp"

using namespace std;


Cube::Cube(int size) {
    for (int i = 0; i < verticesCount; i++) {
        colors[i] = vertices[i];
        vertices[i] = vertices[i] * size;
    }

    manageBuffers();
}

Cube::Cube(int size, float x, float y, float z) {
    for (int i = 0; i < verticesCount; i++) {
        colors[i] = vertices[i];
        vertices[i] = vertices[i] * size;
    }
    move(glm::vec3(x, y, z));
}

Cube::~Cube() {}

void Cube::draw() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Cube::manageBuffers() {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void Cube::move(glm::vec3 vec) {
    for (int i = 0; i < verticesCount; i++) {
        if ((i % 3) == 0) vertices[i] = vertices[i] + vec[0];
        if ((i % 3) == 1) vertices[i] = vertices[i] + vec[1];
        if ((i % 3) == 2) vertices[i] = vertices[i] + vec[2];
    }
    manageBuffers();
}