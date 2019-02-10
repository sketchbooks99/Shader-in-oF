#version 150

#pragma include "3d.glsl"

uniform float time;
uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3)out;

in Vertex {
    in vec3 position;
    in vec3 normal;
    in vec4 color;
    in vec2 texcoord;
} vertex[];

out vec4 vColor;
out vec3 vNormal;

void main() {
    vec3 center = vec3(gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz / 3.0;
    float noise = abs(cnoise(center + time * 0.4));
    for(int i=0; i<gl_in.length(); i++) {
        if(noise > 0.7) {
            vColor = vec4(1.0, 0.0, 0.5, 1.0);
         }else if(noise > 0.5) {
            vColor = vec4(0.0, 1.0, 0.4, 1.0);
        } else {
            vColor = vec4(vec3(1.0-noise), 1.0);
        }
        gl_Position = modelViewProjectionMatrix * vec4(gl_in[i].gl_Position.xyz, 1.0);
        vNormal = vertex[i].normal;
        EmitVertex();
    }
    EndPrimitive();
}
