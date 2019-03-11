#version 150
precision mediump float;

uniform int numTrail;
in vec2 vTexCoord;
in float trailId;

out vec4 fragColor;

void main() {
    fragColor = vec4(trailId / numTrail, 0.0, 0.5, 0.7);
}