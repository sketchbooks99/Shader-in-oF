#version 150

#pragma include "3d.glsl"

uniform float time;
uniform vec3 inColor;
uniform vec3 outColor;
uniform vec3 spColor;

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
    float offset = noiseValue > 0.3 ? 0.4 : noiseValue < -0.3 ? -0.2 : 0.0;
    gl_Position = vec4(position.xyz + normal * offset * 150.0, 1.0);
    vertex.color = noiseValue > 0.3 ? vec4(outColor, .8) : noiseValue < -0.3 ? vec4(inColor,.8) : vec4(spColor, .8);
    vertex.texcoord = texcoord;
}