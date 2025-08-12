#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in float transmittance;
layout(location = 2) in float radius;

uniform mat4 MVP;
uniform vec3 pos;

out float trans;
out float r;
out vec3 p;

void main() {
    gl_Position = MVP * vec4(vertexPosition_modelspace + pos, 1);
    trans = transmittance;
    r = radius;
    p = vertexPosition_modelspace;
}