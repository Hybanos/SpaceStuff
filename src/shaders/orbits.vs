#version 400 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in mat3 base;
layout(location = 5) in vec3 offset;
layout(location = 6) in float major;
layout(location = 7) in float minor;
layout(location = 8) in float anomalies;

uniform mat4 MVP;
out vec4 fragment_color;
flat out float a;

void main() {
    gl_Position =  MVP * vec4(vertexPosition_modelspace * vec3(major, 1, minor) * base + offset, 1);
    fragment_color = vertex_color;
    a = anomalies;
}