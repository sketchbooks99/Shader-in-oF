#version 150
precision mediump float;

uniform sampler2DRect tex;
uniform float time;
uniform int repTime;

in vec2 vTexCoord;

out vec4 fragColor;

// void main() {
//     vec2 coord = vTexCoord * float(repTime) / vec2(1024.0, 768.0);
//     coord = fract(coord) * vec2(1024.0, 768.0);
//     vec3 p = texture(tex, coord).xyz;
//     vec2 st = (coord * 2.0 - vec2(1024.0, 768.0)) / 768.0;
//     float f = clamp(sin(time) / length(st), 0.0, 1.0);
//     fragColor = vec4(p*(1-f) + (vec3(1.0)-p)*f, 1.0);
// }

void main() {
    vec2 coord = vTexCoord;
    vec2 norCoord = (coord * 2.0 - vec2(1024.0, 768.0)) / 768.0;
    coord += length(norCoord)*20.0 * vec2(sin(norCoord.x*3.14), cos(norCoord.y * 3.14));
    vec3 p = texture(tex, vTexCoord).xyz;
    float r = texture(tex, coord).r;
    fragColor = vec4(r, p.gb, 1.0);
}
