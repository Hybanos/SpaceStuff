#version 400 core
#define M_PI 3.1415926535897932384626433832795

in vec4 fragment_color;
flat in float a;

uniform mat4 MVP;

out vec4 color;

void main() {
    float xx = fragment_color.x / (M_PI * 2);
    float aa = a / (M_PI * 2);
    float alpha;

    if (xx - aa > 0) {
        alpha = xx - aa;
    } else {
        alpha = 1 + (xx - aa);
    }

    alpha = max(alpha * alpha * alpha, 0.5);
    color = vec4(0.6, 0.6, 1.0, alpha);
}