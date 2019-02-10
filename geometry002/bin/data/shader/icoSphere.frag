#version 150
precision mediump float;

uniform mat4 invMatrix;

in vec4 vColor;
in vec3 vNormal;

out vec4 fragColor;

const vec3 lightDir = vec3(1.0, 1.0, 0.1);

void main() {
    vec3 ambient = vColor.rgb * 0.7;
    vec3 invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;
    float diffuse = abs(dot(invLight, normalize(vNormal)));
    float specular = pow(diffuse, 20.0);
    vec4 destColor = vec4(ambient, 1.0) + diffuse * vColor + vec4(vec3(specular), 1.0);

    fragColor = vColor;
}
