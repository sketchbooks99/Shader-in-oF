#version 150
precision mediump float;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform float time;

in vec4 position;
in vec4 normal;

out Vertex {
    vec3 position;
    vec3 normal;
}vertex;

void main() {
    vertex.position = position.xyz;
    vertex.normal = normal.xyz;
    gl_Position = position;
}
