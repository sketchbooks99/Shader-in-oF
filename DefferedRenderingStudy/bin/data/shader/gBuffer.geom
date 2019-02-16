#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=6) out;

in Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} vertex[];

out vec3 vNormal;
out vec3 vPosition;
out vec2 vTexCoord;

void main() {
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(b,a));

    for(int i=0; i<gl_in.length(); i++) {
        vNormal = normal;
        vPosition = vertex[i].position;
        gl_Position = gl_in[i].gl_Position;
        vTexCoord = vertex[i].texcoord;
        EmitVertex();
    }
    EndPrimitive();
}