#version 150

uniform sampler2DRect posTex;
uniform int trailLength;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 camPos;
uniform float trailWidth;

layout(triangles) in;
layout(triangle_strip, max_vertices = 22) out;

in Vertex {
    vec3 pos;
    int id;
    vec2 texcoord;
} vertex[];

out float trailId;

void main() {
    trailId = float(vertex[0].id);
    // vec3 dir = vertex[0].dir;
    // vec3 toCamDir = normalize(camPos - dir);
    // vec3 sideDir = normalize(cross(toCamDir, dir));
    float width = trailWidth * 0.5;

    for(int i=0; i<trailLength; i++) {
        vec3 pos = texture(posTex, vertex[0].texcoord + vec2(float(i), 0.0)).xyz * 100.0;
        gl_Position = modelViewProjectionMatrix * vec4(pos + width, 1.0);
        EmitVertex();
        gl_Position = modelViewProjectionMatrix * vec4(pos - width, 1.0);
        EmitVertex();   
    }
    // gl_Position = modelViewProjectionMatrix * vec4(posNext + width, 1.0);
    // EmitVertex();
    // gl_Position = modelViewProjectionMatrix * vec4(posNext - width, 1.0);
    // EmitVertex();

    EndPrimitive();
}
