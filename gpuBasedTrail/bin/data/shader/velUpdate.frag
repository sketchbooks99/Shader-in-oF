#version 150
precision mediump float;

#pragma include "noise4D.frag"

uniform sampler2D posTex;
uniform sampler2D prevVelTex;
uniform float time;
uniform float timestep;

in vec2 vTexCoord;

out vec4 fragColor;

vec3 curlNoise(vec3 p, float id) {
    return vec3(
        snoise(vec4(0.4 * p, id * 0.04 + time * 0.5)),
        snoise(vec4(0.1 * p, id * 0.026 + time * 0.5)),
        snoise(vec4(0.65 * p, id * 0.073 + time * 0.5))
    );
}

void main() {
    if(vTexCoord.x >= 1.0) return;

    float id = vTexCoord.y;
    vec3 pos = texture(posTex, vTexCoord).xyz;
    vec3 vel = texture(prevVelTex, vTexCoord).xyz;

    vel += curlNoise(pos, id) * 50.0;

    fragColor = vec4(pos, 1.0);
    // fragColor = vec4(vTexCoord, 1.0, 1.0);
}