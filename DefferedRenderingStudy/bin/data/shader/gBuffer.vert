#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
    vPosition = (view * model * position).xyz;
    // vNormal = normalize(view * model * vec4(normal, 1.0)).xyz;
    vNormal = normal;
    vTexCoord = texcoord;
    gl_Position = projection * view * model * position;
}