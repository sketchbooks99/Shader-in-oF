#version 150
precision mediump float;

uniform int numTrail;

in vec2 vTexCoord;
in float trailId;
in vec4 vColor;

out vec4 fragColor;

void main() {
    fragColor = vColor;
}