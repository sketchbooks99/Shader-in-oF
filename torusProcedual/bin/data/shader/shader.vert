#version 150

in vec4 position;
in vec3 normal;
in vec4 color;
in vec2 texcoord;

out Vertex {
	vec3 normal;
	vec2 texcoord;
} vertex;

void main() {
	gl_Position = position;
	vertex.normal = normal;
	vertex.texcoord = texcoord;
}