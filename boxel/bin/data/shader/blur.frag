#version 150
precision mediump float;

uniform sampler2DRect tex;
uniform vec2 direction;
uniform float threshold;
uniform float size;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    const int N = 16;
    float delta = size / float(N);

    vec3 color = texture(tex, vTexCoord).rgb;
    if(texture(tex, vTexCoord).a < threshold) color = vec3(0.0);

    for(int i=0; i<N; i++) {
        vec2 d = direction * (float(i) * delta);
        color += texture(tex, vTexCoord + d).rgb;
        color += texture(tex, vTexCoord - d).rgb;
    }

    color /= float(N) * 2 + 1;

    fragColor = vec4(color, 1.0);
}
