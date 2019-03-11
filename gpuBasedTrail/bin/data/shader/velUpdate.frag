#version 150
precision mediump float;

#pragma include "noise4D.frag"

uniform sampler2DRect posTex;
uniform sampler2DRect prevVelTex;
uniform float time;
uniform float timestep;

in vec2 vTexCoord;

out vec4 fragColor;

vec3 curlNoise(vec3 p, float id) {
    return vec3(
        snoise(vec4(0.1 * p, 7.225 + time * 0.5)),
        snoise(vec4(0.1 * p, 3.552 + time * 0.5)),
        snoise(vec4(0.1 * p, 1.259 + time * 0.5))
    );
}

void main() {
    if(vTexCoord.x >= 1.0) discard;

    float id = vTexCoord.y;
    vec3 pos = texture(posTex, vTexCoord).xyz;
    vec3 vel = texture(prevVelTex, vTexCoord).xyz;

    vel += 50.0 * curlNoise(pos, id) * 0.2;
    vel += -pos * length(pos) * 0.1;
    vel *= 0.9 + abs(sin(vTexCoord.y * 9.0)) * 0.03;

    fragColor = vec4(vel, 1.0);
    // fragColor = vec4(vTexCoord, 1.0, 1.0);
}