#version 330 core
precision mediump float;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gColorSpec;
// out vec4 fragColor;

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;

uniform sampler2D normalMap;
uniform int isBump;
uniform vec3 mColor;

void main() {
    gPosition = vec4(vPosition, 1.0);
    if(isBump == 1) {
        gNormal = texture(normalMap, vTexCoord);
        gColorSpec = vec4(vec3(.0), 1.0);
    } else {
        gNormal = vec4(vNormal, 1.0);
        gColorSpec = vec4(mColor, 0.0);        
    }
}