#version 150
precision mediump float;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3)out;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 vpMatrix;
uniform mat4 modelMatrix;
uniform bool mirror;

in Vertex {
    vec3 normal;
    vec3 position;
} vertex[];

out vec3 vNormal;
out vec3 vPosition;
out vec4 vColor;

void main() {
    vec3 a = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
    vec3 normal = normalize(cross(b,a));
    vec3 center = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz) / 3.0;

    for(int i=0; i<gl_in.length(); i++) {
        vNormal = normal;
        vec3 newVertex = (modelMatrix * gl_in[i].gl_Position).xyz;
        if(mirror) newVertex.y = -newVertex.y;
        gl_Position = vpMatrix * vec4(newVertex, 1.0);
        vPosition = vertex[i].position;
        EmitVertex();
    }
    EndPrimitive();
}
