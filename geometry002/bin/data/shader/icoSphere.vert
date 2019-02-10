#version 150

uniform float time;

in vec4 position;
in vec4 normal;
in vec2 texcoord;
in vec4 color;

out Vertex {
    vec3 normal;
    vec2 texcoord;
    vec4 color;
    vec3 position;
} vertex;

void main() {
    vertex.normal = normal.xyz;
    vertex.texcoord = texcoord;
    vertex.color = color;
    vertex.position = position.xyz;
    gl_Position = position;
}
