#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;
uniform mat3 rota;

out vec3 vertex_coords;

void main() {
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    vertex_coords = (vertexPosition_modelspace);
}