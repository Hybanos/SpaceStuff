#version 330 core
out vec3 color;
in vec3 fragmentColor;

void main() {
    // color = vec3(1, 1, 1);
    color = fragmentColor;
}