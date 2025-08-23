#version 330 core

// in float trans;
// in float r;
in vec3 p;

out vec4 color;

uniform sampler2D tex;
uniform mat4 MVP;
uniform vec3 pos;
uniform mat3 rota;
uniform vec2 range;
uniform float transmittance;
uniform int selected;
uniform int bitset[16];

// uniform vec2 highlight_bounds;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453 / 100000);
}

void main() {
    vec3 tmp = vec3(1.0, 1.0, 0.90);
    float dist = distance(p, vec3(0));
    if (dist < range.x || dist > range.y) discard;

    float percent = (dist - range.x) / (range.y - range.x);
    int bit = int(percent * 512);
    int i = bitset[bit / 32];
    if ((i & (1 << (bit % 32))) == 0) discard;

    float a = dist / 10000;

    float offset = 0.5 + fract(dist) / 500;
    float t = texture(tex, vec2(fract(dist / 25000), offset)).x;
    t = t * (1 - transmittance);
    t = max(0.005, t);
    color = vec4(tmp, t);
    if (selected != 0) color = vec4(1);
}