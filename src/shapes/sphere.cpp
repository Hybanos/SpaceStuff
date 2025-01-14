#include "sphere.hpp"

using namespace std;

// Sphere::Sphere(int i) {
//     int stacks = 10;
//     int sectors = 10;
//     int radius = 1;

//     vector<GLfloat> v;

//     for (int i = 0; i <= stacks; i++) {
//         for (int j = 0; j <= sectors; j++) {
//             GLfloat t = j * 2 * PI / sectors; 
//             GLfloat p = PI / 2 - i * (PI / stacks);

//             GLfloat x = radius * cosf(p) * cosf(t);
//             GLfloat y = radius * cosf(p) * sinf(t);
//             GLfloat z = radius * sinf(p);

//             v.push_back(x);
//             v.push_back(y);
//             v.push_back(z);
//         }
//     }
    
//     // for (int i = 0; i < v.size(); i++) {
//     //     vertices[i] = v[i];
//     //     colors[i] = v[i];
//     //     cout << vertices[i] << endl;
//     // }
//     vector<int> indices;
//     vector<int> lineIndices;
//     for (int i = 0; i < stacks; i++) {
//         int k1 = i * (sectors + 1);
//         int k2 = k1 + sectors + 1;
//         for (int j = 0; j < sectors; j++, k1++, k2++) {
//             if (i != 0) {
//                 indices.push_back(k1);
//                 indices.push_back(k2);
//                 indices.push_back(k1 + 1);
//             }

//             if (i != (stacks - 1)) {
//                 indices.push_back(k1+1);
//                 indices.push_back(k2);
//                 indices.push_back(k2+1);
//             }

//             lineIndices.push_back(k1);
//             lineIndices.push_back(k2);
//             if (i != 0) {
//                 lineIndices.push_back(k1);
//                 lineIndices.push_back(k1+1);
//             }
//         }
//     }
    
//     for (int i = 0; i < 363; i = i + 3) {
//         vertices[i] = v[indices[i]];
//         vertices[i+1] = v[indices[i+1]];
//         vertices[i+2] = v[indices[i+2]];
//         cout << indices[i] << " " << lineIndices[i] << endl;
//     }

//     cout << verticesCount  << " " << indices.size() << " " << lineIndices.size() << endl;
//     manageBuffers();
// }

Sphere::Sphere(int i) {

    

    manageBuffers();
}

void Sphere::draw() {
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

void Sphere::manageBuffers() {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}
