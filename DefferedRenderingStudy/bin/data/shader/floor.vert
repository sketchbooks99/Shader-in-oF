#version 330 core

#pragma include "3d.glsl"

in vec4 position;
in vec3 normal;
in vec4 color; 
in vec2 texcoord;

out Vertex {
    // vec4 position;
    vec3 normal;
    vec4 color;
    vec2 texcoord;
} vertex;

void main() {
    vec3 newVertex = position.xyz;
    newVertex.z += snoise(position.xyz);
    gl_Position = vec4(newVertex, 1.0);
    vertex.normal = normal;
    vertex.texcoord = texcoord;
    vertex.color = color;
}