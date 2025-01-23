#version 400 core
#define M_PI 3.1415926535897932384626433832795

in vec3 fragment_coords;
in vec4 fragment_color;

uniform mat4 MVP;
uniform mat3 base;
uniform vec3 offset;
uniform float a;
uniform float b;

out vec4 color;

void main() {

    vec4 color_1 = vec4(0.6, 0.6, 1.0, 1.0);
    vec4 color_2 = vec4(0.6, 0.6, 1.0, 0.0);

    float xx = fragment_color.x / (M_PI * 2);
    float aa = a / (M_PI * 2);
    float alpha;

    if (xx - aa > 0) {
        alpha = xx - aa;
    } else {
        alpha = 1 + (xx - aa);
    }

    color = vec4(0.6, 0.6, 1.0, alpha);
    // color = vec4(alpha, 0.0, 0.0, 1.0);

}