#version 150

uniform mat4 modelViewProjectionMatrix;
uniform float scale;

layout(triangles) in;
layout(triangle_strip, max_vertices=36) out;

in Vertex {
    vec4 color;
    vec2 texcoord;
} vertex[];

out vec3 vNormal;
out vec4 vColor;

void createTriangle(vec3 v0, vec3 v1, vec3 v2, int face) {
    vec3 faceNormal = vec3(0.0);
    switch(face){
        case 0: // positive X
            faceNormal = vec3(1.0, 0.0, 0.0);
            break;
        case 1: // negative X
            faceNormal = vec3(-1.0, 0.0, 0.0);
            break;
        case 2: // positive Y
            faceNormal = vec3(0.0, 1.0, 0.0);
            break;
        case 3: // negative Y
            faceNormal = vec3(0.0, -1.0, 0.0);
            break;
        case 4: // positive Z
            faceNormal = vec3(0.0, 0.0, 1.0);
            break;
        case 5: // negative Z
            faceNormal = vec3(0.0, 0.0, -1.0);
            break;
    }
    gl_Position = modelViewProjectionMatrix * vec4(v0, 1.0);
    vNormal = faceNormal;
    vColor = vertex[0].color;
    EmitVertex();
    gl_Position = modelViewProjectionMatrix * vec4(v1, 1.0);
    vNormal = faceNormal;
    vColor = vertex[0].color;
    EmitVertex();
    gl_Position = modelViewProjectionMatrix * vec4(v2, 1.0);
    vNormal = faceNormal;
    vColor = vertex[0].color;
    EmitVertex();
    EndPrimitive();
}

void main() {
    vec3 v1 = gl_in[0].gl_Position.xyz + vec3(-1, 1, 1) * scale;
    vec3 v2 = gl_in[0].gl_Position.xyz + vec3(-1, -1, 1) * scale;
    vec3 v3 = gl_in[0].gl_Position.xyz + vec3(1, -1, 1) * scale;
    vec3 v4 = gl_in[0].gl_Position.xyz + vec3(1, 1, 1) * scale;
    vec3 v5 = gl_in[0].gl_Position.xyz + vec3(-1, 1, -1) * scale;
    vec3 v6 = gl_in[0].gl_Position.xyz + vec3(-1, -1, -1) * scale;
    vec3 v7 = gl_in[0].gl_Position.xyz + vec3(1, -1, -1) * scale;
    vec3 v8 = gl_in[0].gl_Position.xyz + vec3(1, 1, -1) * scale;
    // ===== xSide =====
    // positive
    createTriangle(v8, v4, v3, 0);
    createTriangle(v8, v3, v7, 0);
    // negative
    createTriangle(v1, v5, v6, 1);
    createTriangle(v1, v6, v2, 1);

    // ===== ySide =====
    // positive
    createTriangle(v8, v5, v1, 2);
    createTriangle(v8, v1, v4, 2);
    // negative
    createTriangle(v6, v7, v3, 3);
    createTriangle(v6, v3, v2, 3);

    // ===== zSide =====
    // positive
    createTriangle(v4, v1, v2, 4);
    createTriangle(v4, v2, v3, 4);
    // negative
    createTriangle(v5, v8, v7, 5);
    createTriangle(v5, v7, v6, 5);
}