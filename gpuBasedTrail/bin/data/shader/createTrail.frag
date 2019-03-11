#version 150
precision mediump float;

uniform int numTrail;
in vec2 vTexCoord;
in float vId;

out vec4 fragColor;

void main() {
    fragColor = vec4(vId / numTrail, 0.5, 0.2, 0.8);
}