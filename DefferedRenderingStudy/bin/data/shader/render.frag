#version 330 core 
precision mediump float;

in vec2 vTexCoord;

uniform sampler2DRect gPosition;
uniform sampler2DRect gNormal;
uniform sampler2DRect gColorSpec;
uniform int debugMode;

out vec4 fragColor;

void main() {
    if(debugMode == 0) {
        fragColor = texture(gPosition, vTexCoord);
    }
    else if(debugMode == 1) {
        fragColor = texture(gNormal, vTexCoord);
    }
    else if(debugMode == 2) {
        fragColor = texture(gColorSpec, vTexCoord);
    } else {
        fragColor = vec4(vTexCoord, 0.0, 1.0);
    }
}