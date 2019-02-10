#version 150
precision mediump float;

uniform mat4 modelViewProjectionMatrix;

in vec3 vNormal;
in vec4 vColor;

out vec4 fragColor;

const vec3 lightDir = vec3(10.0, 10.0, 1.0);

void main() {
    vec4 ambient = vColor * 0.2;
    mat4 invMat = inverse(modelViewProjectionMatrix);
    vec3 invLight = normalize(invMat * vec4(lightDir, 0.0)).xyz;
    float diffuse = abs(dot(normalize(vNormal), invLight));
    float specular = pow(diffuse, 10.0);
    fragColor = ambient + vec4(vec3(diffuse * vColor.rgb + vec3(specular)), 1.0);
}
