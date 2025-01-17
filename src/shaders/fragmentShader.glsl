#version 330 core
out vec4 color;
in vec3 fragmentColor;

in vec3 texture_dir;
uniform samplerCube cubemap; // cubemap texture sampler

void main() {
    // color = vec3(1, 1, 1);
    // color = fragmentColor;
    color = texture(cubemap, texture_dir) * vec4(fragmentColor, 1.0);;
}