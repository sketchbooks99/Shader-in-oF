#version 150

uniform mat4 modelViewProjectionMatrix;
uniform float time;

#pragma include "3d.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices = 90) out;

in Vertex {
    vec3 normal;
    vec2 texcoord;
    vec4 color;
    vec3 position;
} vertex[];

out vec3 vNormal;
out vec4 vColor;

void main() {
    vec3 a = vec3(vertex[1].position - vertex[0].position);
    vec3 b = vec3(vertex[2].position - vertex[0].position);
    vec3 normal = normalize(cross(b, a));
    vec3 center = (vertex[0].position + vertex[1].position + vertex[2].position) / 3.0;
    float noise = cnoise(center + time);
    float diffHeight = 5.0;
    int maxIndex = 2 + int(floor(abs(noise)*30.0));

    for(int pr_i = 0; pr_i < maxIndex; pr_i++) {
        for(int i=0; i<gl_in.length(); i++) {
            float offset = float(pr_i) * 0.04;
            vec3 newVertex = gl_in[i].gl_Position.xyz * (1.0 - offset) + center * offset;
            newVertex += normal * diffHeight * float(pr_i);
            gl_Position = modelViewProjectionMatrix * vec4(newVertex, 1.0);
            // vColor = pr_i == 0 ? vec4(0.7,0.4,1.0,1.0) : vec4(1.0, 1.0 - offset, 0.0, 1.0);
            if(noise > 0.6) {
                vColor = vec4(offset, 0.0, offset*0.5, 1.0);
            } else if(noise > 0.4) {
                vColor = vec4(0.0, offset, offset * 0.4, 1.0);
            } else {
                vColor = vec4(vec3(offset)*2.0, 1.0);
            }
            vNormal = normal;
            EmitVertex();
        }
        EndPrimitive();
    }
}
