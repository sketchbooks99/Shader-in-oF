#version 150
precision mediump float;

uniform mat4 invMatrix;

in vec3 vNormal;
in vec4 vColor;

out vec4 fragColor;

const vec3 lightPos = vec3(1.0, 1.0, 0.0);

void main() {
    // lighting on
    vec3 invLight = normalize(invMatrix * vec4(lightPos, 0.0)).xyz;
    float diffuse = clamp(dot(vNormal, invLight), 0.3, 1.0);
    float specular = pow(diffuse, 30.0);
    vec3 destColor = vColor.rgb * .3 + vColor.rgb * diffuse * .7 + specular;
    // fragColor = vec4(destColor, vColor.a);
    // fragColor = vec4(abs(vNormal), 1.0);

    //lighting off
    fragColor = vColor;
}