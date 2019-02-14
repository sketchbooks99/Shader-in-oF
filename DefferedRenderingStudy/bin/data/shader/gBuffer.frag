#version 330 core
precision mediump float;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gColorSpec;
// out vec4 fragColor;

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;

void main() {
    gPosition = vPosition;
    gNormal = normalize(vNormal);
    gColorSpec = vec4(vTexCoord, 1.0, 1.0);
    // fragColor = vec4(1.0,0.0,0.0,1.0);
}