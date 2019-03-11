#version 150
precision mediump float;

uniform sampler2DRect posTex;
uniform sampler2DRect velTex;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    // vec3 pos = texture(posTex, vTexCoord).xyz;
    // vec3 pos = texture(velTex, vTexCoord).xyz;

    vec3 pos = vec3(1.0);
    if(vTexCoord.x <= 1.0) {
        pos = texture(posTex, vTexCoord).xyz;
        vec3 vel = texture(velTex, vTexCoord).xyz;

        pos += vel * 0.03;
    } else {
        vec2 uv = vTexCoord - vec2(1.0, 0.0);
        pos = texture(posTex, uv).xyz;
    }
    fragColor = vec4(pos, 1.0);
    // fragColor = vec4(vTexCoord, 1.0, 1.0);
}