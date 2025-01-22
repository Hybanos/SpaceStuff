#version 330 core
in vec4 fragmentColor;

out vec4 color;

void main() {
    color = fragmentColor;
    if (color.a < 0.0) discard;
}