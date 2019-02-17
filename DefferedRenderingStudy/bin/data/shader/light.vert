#version 330 core

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

in vec4 position;
in vec3 normal;

out vec3 vNormal;
out vec3 vPosition;

void main() {
    gl_Position = projection * view * model * position;
    vPosition = (model * position).xyz;
    vNormal = normalize(normal);
}