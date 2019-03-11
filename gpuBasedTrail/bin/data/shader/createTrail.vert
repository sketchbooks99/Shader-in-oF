#version 150

uniform mat4 modelViewProjectionMatrix;
uniform sampler2DRect posTex;
uniform int trailLength;
uniform vec2 texSize;

in vec3 normal;
in vec2 texcoord;
in vec4 color;

// out vec3 vPos;
// out vec2 vTexCoord;
// out float trailId;

// void main() {
//     vec2 coord = vec2(gl_VertexID, gl_InstanceID);
//     vec3 pos = texture(posTex, coord).xyz * 40.0;
//     // vec3 pos = vec3(gl_VertexID, gl_InstanceID * 0.1, 0.0);
//     trailId = gl_InstanceID;
//     vTexCoord = coord / texSize;
//     gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
// }

// ========== use Geometry shader ==========
out Vertex {
    vec3 pos;
    int id;
    vec2 texcoord;
}vertex;

void main() {
    if(gl_VertexID == 0) {
        vec2 coord = vec2(gl_VertexID, gl_InstanceID);
        vec3 pos = texture(posTex, coord).xyz * 100.0;
        vec3 posNext = texture(posTex, coord + vec2(1.0, 0.0)).xyz * 100.0;
        vertex.pos = pos;
        vertex.id = gl_InstanceID;
        vertex.texcoord = coord;
        gl_Position = vec4(pos, 1.0);
    } else {
        return;
    }
}