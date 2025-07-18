#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;
uniform mat3 rota;
uniform vec3 pos;

out vec3 vertex_coords;

void main() {
    gl_Position = MVP * vec4(vertexPosition_modelspace + pos, 1);
    vertex_coords = (vertexPosition_modelspace);
}