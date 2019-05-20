#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvpMatrix;
uniform float time;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
	vPosition = (view * model * position).xyz;
	vNormal = normal;
	vTexCoord = texcoord;
	gl_Position = mvpMatrix * position;
}