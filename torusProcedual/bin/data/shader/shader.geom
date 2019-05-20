#version 150

uniform mat4 mvpMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in Vertex {
	vec3 normal;
	vec2 texcoord;
} vertex[];

out vec3 vNormal;
out vec4 vColor;

void main() {
	vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz / 3.0;
	vec3 normal = normalize(cross(b, a));

	for(int i=0; i<gl_in.length; i++) {
		gl_Position = mvpMatrix * gl_in[i].gl_Position;
		vNormal = vertex[i].normal;
		vColor = vec4(vertex[i].texcoord, 0.5, 1.0);
		EmitVertex();
	}
	EndPrimitive();

	for(int i=0; i<gl_in.length; i++) {
		vec3 newVertex = gl_in[i].gl_Position.xyz * 0.9 + center * 0.1 + normal * 0.1;
		gl_Position = mvpMatrix * vec4(newVertex, 1.0);
		vNormal = vertex[i].normal;
		vColor = vec4(1.0);
		EmitVertex();
	}
	EndPrimitive();
}