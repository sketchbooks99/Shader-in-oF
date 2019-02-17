#version 330 core

#pragma include "3d.glsl"

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int isBump;
uniform float time;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} vertex;


void main() {
    vec3 newVertex = position.xyz;
    if(isBump == 0) {
        newVertex.z -= snoise(vec3(position.xyz * 5.0) + time * 0.2) * 40.0;
    } else if(isBump == 1) {
        newVertex += snoise(vec3(position).xyz + time * 0.1) * normal * 10.0;
    }
    vertex.position = (view * model * vec4(newVertex,1.0)).xyz;
    // vNormal = normalize(view * model * vec4(normal, 1.0)).xyz;
    vertex.normal = normal;
    vertex.texcoord = texcoord;
    gl_Position = projection * view * model * vec4(newVertex,1.0);
}


// vertex & fragment version
// out vec3 vPosition;
// out vec3 vNormal;
// out vec2 vTexCoord;

// void main() {
//     vec3 newVertex = position.xyz;
//     if(isBump == 0) {
//         newVertex.z -= snoise(vec3(position.xyz * 10.0)) * 10.0;
//     }
//     vPosition = (view * model * vec4(newVertex,1.0)).xyz;
//     // vNormal = normalize(view * model * vec4(normal, 1.0)).xyz;
//     vNormal = normal;
//     vTexCoord = texcoord;
//     gl_Position = projection * view * model * vec4(newVertex,1.0);
// }