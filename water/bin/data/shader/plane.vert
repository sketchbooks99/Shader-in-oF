#version 150

#pragma include "util.frag"
#pragma include "noise4D.frag"
#pragma include "noise2d.glsl"

uniform mat4 modelViewProjectionMatrix;
uniform float time;

in vec4 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

out vec3 vPosition;
out vec3 vNormal;
out vec4 vColor;

vec3 offset = vec3(-0.00001, 0.0, 0.00001);

void main() {
    vec3 newVertex = position.xyz;
    float negaXnoise = snoise(texcoord * 4.0 + offset.xy + time * 0.2);
    float posiXnoise = snoise(texcoord * 4.0 + offset.zy + time * 0.2);
    float negaZnoise = snoise(texcoord * 4.0 + offset.yx + time * 0.2);
    float posiZnoise = snoise(texcoord * 4.0 + offset.yz + time * 0.2);
    float noise = snoise(texcoord * 4.0 + time * 0.2);
    newVertex.y += noise * abs(normal.z) * 100.0;

    vec3 aN = normalize(vec3(offset.z * 2.0, posiXnoise - negaXnoise, 0.0));
    vec3 bN = normalize(vec3(0.0, posiZnoise - negaZnoise, offset.z * 2.0));
    vec3 newNorm = normalize(cross(bN, aN));

    // vec3 aN = vec3();
    // vNormal = normal.xyz;
    vNormal = newNorm;
    vColor = vec4(newNorm.xyz, 1.0);
    vPosition = newVertex.xyz;
    gl_Position = modelViewProjectionMatrix * vec4(newVertex, 1.0);
}
