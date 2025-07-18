#version 400 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in mat3 base;
layout(location = 5) in vec3 offset;
layout(location = 6) in float major;
layout(location = 7) in float minor;
layout(location = 8) in float anomalies;
layout(location = 9) in float flag;

uniform mat4 MVP;
uniform vec3 pos;

out vec4 fragment_color;
flat out float a;
flat out float f;

void main() {
    gl_Position =  MVP * vec4(vertexPosition_modelspace * vec3(major, 1, minor) * base + offset + pos, 1);
    fragment_color = vertex_color;
    a = anomalies;
    f = flag;
}