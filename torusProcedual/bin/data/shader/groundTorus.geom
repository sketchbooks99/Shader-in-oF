#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 9) out;

uniform mat4 mvpMatrix;
uniform float time;

in Vertex {
	vec3 normal;
	vec2 texcoord;
} vertex[];

out vec4 vColor;
out vec3 vNormal;

void main() {
	vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz / 3.0;
	vec3 normal = normalize(cross(b, a));

	for(int i = 0; i < gl_in.length; i++) {
		gl_Position = mvpMatrix * gl_in[i].gl_Position;
		vColor = vec4(1.0, 0.6, 0.1, 1.0);
		vNormal = abs(vertex[i].normal);
		EmitVertex();
	}
	EndPrimitive();

	for(int j = 0; j < 2; j++) {
		for(int i = 0; i < gl_in.length; i++) {
			vec3 newVertex = gl_in[i].gl_Position.xyz * 0.9 + center * 0.1 - normal * (j == 0 ? 0.5 : -0.5);
			gl_Position = mvpMatrix * vec4(newVertex, 1.0);
			vec4 baseColor = vec4(0.1, 0.0, 0.2, 1.0);
			vColor = baseColor;
	//		vColor = vec4(vertex[i].texcoord, 1.0, 1.0);
			vNormal = abs(vertex[i].normal);
			EmitVertex();
		}
		EndPrimitive();
	}
}