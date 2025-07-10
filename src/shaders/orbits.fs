#version 400 core
#define M_PI 3.1415926535897932384626433832795

in vec4 fragment_color;
flat in float a;
flat in float f;

uniform mat4 MVP;

out vec4 color;

void main() {

    vec3 normal_color = vec3(0.6, 0.6, 1.0);
    vec3 filtered_color = vec3(0.8, 0.6, 0.6);
    vec3 selected_color = vec3(1.0, 0.6, 0.6);

    float xx = fragment_color.x / (M_PI * 2);
    float aa = a / (M_PI * 2);
    float alpha;

    if (xx - aa > 0) {
        alpha = xx - aa;
    } else {
        alpha = 1 + (xx - aa);
    }

    alpha = max(alpha * alpha * alpha, 0.5);

    if (f == 0) discard;
    if (f == 1) color = vec4(normal_color, alpha);
    if (f == 2) color = vec4(selected_color, alpha);
}