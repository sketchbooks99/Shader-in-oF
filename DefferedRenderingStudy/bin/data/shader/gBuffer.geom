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
out vec3 vColor;

uniform int isBump;
uniform vec3 mColor;

void main() {
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(b,a));
    vec3 center = ( gl_in[2].gl_Position + gl_in[1].gl_Position + gl_in[0].gl_Position ).xyz / 3.0 ;

    for(int i=0; i<gl_in.length(); i++) {
        vNormal = normal;
        vPosition = vertex[i].position;
        gl_Position = gl_in[i].gl_Position;
        vTexCoord = vertex[i].texcoord;
        vColor = mColor;
        EmitVertex();
    }
    EndPrimitive();

    for(int i=0; i<gl_in.length(); i++) {
        vNormal = normal;
        vec3 newVertex = gl_in[i].gl_Position.xyz * .9 + center * .1;
        newVertex += normal * 10.0;
        vec3 vNewVertex = vertex[i].position * .9 + center * .1;
        vNewVertex += normal * 10.0;
        vPosition = vNewVertex;
        gl_Position = vec4(newVertex, 1.0);
        vTexCoord = vertex[i].texcoord;
        vColor = vec3(1.0) - mColor;
        EmitVertex();
    }
    EndPrimitive();
}