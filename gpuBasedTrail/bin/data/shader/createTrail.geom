#version 150

uniform sampler2D velTex;
uniform sampler2D posTex;
uniform mat4 modelViewProjectionMatrix;

layout(points) in
layout(triangle_strip, maxvertices = 4) out;

in Vertex {
    vec4 position;
    vec3 normal;
    vec2 texcoord;
    vec4 color;
} vertex[];

out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vColor;

void main() {
    int id = gl_InstanceID;
    int vertId = gl_VertexID;

}
