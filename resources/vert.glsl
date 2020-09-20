#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform vec3 eye;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpace;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 LightDirection;
    vec3 EyeDirection;
    vec4 ShadowCoord;
} vs_out;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1.0);
    vs_out.FragPos = (model * vec4(pos, 1)).xyz;
    vs_out.ShadowCoord = lightSpace * vec4(pos, 1);
    vs_out.EyeDirection = eye - (view * model * vec4(pos, 1)).xyz;
    vs_out.LightDirection = (view * vec4(80.0, 100.0, 80.0, 0)).xyz;
    vs_out.Normal = (view * model * vec4(normal, 0)).xyz;
    vs_out.TexCoords = texcoord;
}