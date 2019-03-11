#version 150
precision mediump float;

uniform sampler2DRect tex;
uniform vec2 direction;
uniform float blurSize;
                                  
in vec2 vTexCoord;
                                  
out vec4 fragColor;
                                  
void main() {
    const int N = 16;
    float delta = blurSize / float(N);
    vec3 color = texture(tex, vTexCoord).rgb;
                                        
    for(int i=0; i<N; i++) {
        vec2 d = direction * float(i) * delta;
        color += texture(tex, vTexCoord + d).rgb;
        color += texture(tex, vTexCoord - d).rgb;
    }
    color /= float(N) * 2 + 1;
                                        
    fragColor = vec4(color, 1.0);
}