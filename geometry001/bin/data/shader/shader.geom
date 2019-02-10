#version 150

uniform float size;
uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in Vertex {
    vec4 color;
    vec2 texcoord;
    vec3 normal;
}vertex[];

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vNormal;

void main() {
    vec3 a = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
    vec3 normal = normalize(cross(b, a));
    vec3 center = vec3(gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz) / 3.0;

    for(int k=0; k<2; k++) {
        for(int i=0; i<gl_in.length(); i++) {
            if(k==0) {
                gl_Position = modelViewProjectionMatrix * vec4(gl_in[i].gl_Position.xyz, 1.0);
                vNormal = normal;
                vColor = vec4(1.0, 0.3, 0.8, 1.0);
                EmitVertex();
            } else {
                vec3 smallPos = gl_in[i].gl_Position.xyz * size + center * (1.0 - size);
                vec3 newPos = smallPos + normal * 0.01f;
                gl_Position = modelViewProjectionMatrix * vec4(newPos, 1.0);
                vNormal = normal;
                vColor = vec4(0.1, 0.0, 0.3, 1.0);
                EmitVertex();
            }
        }
        EndPrimitive();
    }
}
