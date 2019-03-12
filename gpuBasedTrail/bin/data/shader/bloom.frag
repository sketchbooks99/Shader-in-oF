#version 150

uniform sampler2DRect origin;
uniform sampler2DRect result;
uniform vec2 resolution;
uniform float strength;
                                  
in vec2 vTexCoord;
                                  
out vec4 fragColor;
                                  
void main() {
    vec2 p = (vTexCoord * 2.0 - resolution) / min(resolution.x, resolution.y);
    vec2 rOffset = vec2(2.0, 0.0);
    vec2 bOffset = vec2(4.0, 0.0);
    vec4 ori = texture(origin, vTexCoord);
    vec4 res = texture(result, vTexCoord);

    float rOri = texture(origin, vTexCoord + rOffset).r;
    float rRes = texture(result, vTexCoord + rOffset).r;
    float bOri = texture(origin, vTexCoord + bOffset).b;
    float bRes = texture(result, vTexCoord + bOffset).b;

    vec3 destColor = vec3(rOri, ori.g, bOri) + vec3(rRes, res.g, bRes) * strength;
    fragColor = vec4(destColor, 1.0);
}