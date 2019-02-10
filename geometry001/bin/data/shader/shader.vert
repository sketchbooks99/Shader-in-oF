#version 150

in vec4 position;
in vec2 texcoord;
in vec4 normal;
in vec4 color;

out Vertex {
    vec4 color;
    vec2 texcoord;
    vec3 normal;
}vertex;

void main() {
    gl_Position = position;
    vertex.color = color;
    vertex.texcoord = texcoord;
    vertex.normal = normal.xyz;
}
