#version 150

#pragma include "3d.glsl"

uniform float time;

in vec4 position;
in vec4 color;
in vec2 texcoord;
in vec3 normal;

out Vertex {
    vec2 texcoord;
    vec4 color;
} vertex;

void main() {
    float noiseValue = cnoise(position.xyz * 0.01 + time);
    float offset = noiseValue > 0.5 ? 0.5 : noiseValue < -0.3 ? 0.3 : 0.0;
    gl_Position = vec4(position.xyz + normal * offset * 150.0, 1.0);
    vertex.color = noiseValue > 0.5 ? vec4(vec3(1.0), 1.0) : noiseValue < -0.3 ? vec4(vec3(0.3),1.0) : vec4(0.3, 0.8, 0.8, 2.0);
    vertex.texcoord = texcoord;
}