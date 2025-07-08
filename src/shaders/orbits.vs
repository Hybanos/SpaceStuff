#version 400 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in mat3 base;

uniform mat4 MVP;
// uniform mat3 base;
// uniform vec3 offset;
// uniform float a;
// uniform float b;

out vec3 fragment_coords;
out vec4 fragment_color;

void main() {
    gl_Position =  MVP * vec4(vertexPosition_modelspace * base, 1);
    fragment_coords = vertexPosition_modelspace * base;
    fragment_color = vertex_color;
}