#version 150
precision mediump float;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    fragColor = vec4(vTexCoord, 1., 1.);
}