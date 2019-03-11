#version 150

uniform sampler2DRect origin;
uniform sampler2DRect result;
uniform float strength;
                                  
in vec2 vTexCoord;
                                  
out vec4 fragColor;
                                  
void main() {
    vec4 ori = texture(origin, vTexCoord);
    vec4 res = texture(result, vTexCoord);
                                        
    fragColor = vec4((ori + res * strength).rgb, 1.0);
}