#version 330 core

in float trans;
in float r;
in vec3 p;

out vec4 color;

uniform sampler2D tex;
uniform vec2 highlight_bounds;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453 / 100000);
}

void main() {
    vec3 tmp = vec3(1.0, 1.0, 0.90);

    float t = 0.8;

    float a = r / 10000;
    t = rand(vec2(a, 0.0));

    float offset = 0.5 + fract(r) / 50;
    t = texture(tex, vec2(fract(r / 25000), offset)).x;
    t = t * (1 - trans);
    t = max(0.005, t);
    color = vec4(tmp, t);
    if (r > highlight_bounds.x && r < highlight_bounds.y) color = vec4(1); 
}