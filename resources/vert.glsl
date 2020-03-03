#version 330 core
layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 col;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 Color;

void main() {
    Color = col;
    gl_Position = proj * view * model * vec4(vert, 1.0);
}