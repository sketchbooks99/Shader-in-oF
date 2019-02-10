#version 150
precision mediump float;

uniform sampler2DRect blurTex;
uniform sampler2DRect renderTex;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    vec4 blur = texture(blurTex, vTexCoord);
    vec4 render = texture(renderTex, vTexCoord);

    fragColor = blur + render;
}
