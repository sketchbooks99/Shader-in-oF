#version 150

uniform mat4 modelViewProjectionMatrix;
uniform sampler2DRect posTex;
uniform vec2 texSize;

in vec3 normal;
in vec2 texcoord;
in vec4 color;

// out Vertex {
//     vec3 pos;
//     vec3 dir;
//     vec3 nextPos;
//     vec3 nextDir;
// }vertex;

out vec3 vPos;
out vec2 vTexCoord;
out float vId;

void main() {
    vec2 coord = vec2(gl_VertexID, gl_InstanceID);
    vec3 pos = texture(posTex, coord).xyz * 40.0;
    // vec3 pos = vec3(gl_VertexID, gl_InstanceID * 0.1, 0.0);
    vId = gl_InstanceID;
    vTexCoord = coord / texSize;
    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
}