#version 330 core
precision mediump float;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gColor;

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;
in vec3 vColor;

uniform vec3 mColor;

void main() {
	gPosition = vec4(vPosition, 1.0);
	gNormal = vec4(vNormal, 1.0);
	gColor = vec4(0.6, 0.3, 0.5, 1.0);
}