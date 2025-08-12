#version 330 core

in float trans;
in float r;
in vec3 p;

out vec4 color;

uniform sampler2D tex;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453 / 100000);
}

void main() {
    vec3 tmp = vec3(1.0, 1.0, 0.90);

    float t = trans;

    // t = 1 * rand(vec2(1, r)); 
    // t = fract(r); 
    float a = r / 10000;
    t = rand(vec2(a, 0.0));
    // t = fract(sqrt(p.x * p.x + p.z * p.z));

    t = texture(tex, vec2(fract(r / 90000), 0.5)).x;
    t = t - 0.35;
    t = t * 2;
    color = vec4(tmp, t);

}