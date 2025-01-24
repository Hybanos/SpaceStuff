#version 330 core
in vec3 fragment_coords;

uniform mat4 MVP;

out vec4 color;

void main() {

    float alpha = 0;

    if      (mod(fragment_coords.x + 10, 1000) < 20 && fragment_coords.y == 0) color = vec4(0.5, 0.5, 0.5, 1);
    else if (mod(fragment_coords.z + 10, 1000) < 20 && fragment_coords.y == 0) color = vec4(0.5, 0.5, 0.5, 1);

    else if (abs(fragment_coords.z) < 30 && fragment_coords.x > 0 && abs(fragment_coords.y) < 30) color = vec4(1, 0, 0, 1);
    else if (abs(fragment_coords.x) < 30 && fragment_coords.y > 0 && abs(fragment_coords.z) < 30) color = vec4(0, 1, 0, 1);
    else if (abs(fragment_coords.x) < 30 && fragment_coords.z > 0 && abs(fragment_coords.y) < 30) color = vec4(0, 0, 1, 1);
    else discard;
}