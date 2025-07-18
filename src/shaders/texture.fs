#version 330 core
in vec3 vertex_coords;
out vec4 color;

uniform samplerCube cubemap; // cubemap texture sampler
uniform mat3 rota;
uniform int flip; // skybox needs to be flipped
uniform vec3 pos;

void main() {
    if (flip != 0) color = texture(cubemap, vertex_coords.zyx * rota);
    else color = texture(cubemap, vertex_coords * rota);
}