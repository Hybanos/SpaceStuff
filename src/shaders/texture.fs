#version 330 core
in vec3 vertex_coords;
out vec4 color;

uniform samplerCube cubemap; // cubemap texture sampler

void main() {
    color = texture(cubemap, vertex_coords);
}