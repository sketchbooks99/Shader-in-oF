#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec3 normal;
in vec2 texcoord;
in vec4 color;

out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = modelViewProjectionMatrix * position;
    vNormal = normal;
    vTexCoord = texcoord;
    vColor = color;
}