#version 150
precision mediump float;

uniform mat4 invMatrix;
uniform samplerCube cubeTexture;
uniform float time;
uniform vec3 eyePosition;

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;

out vec4 fragColor;

const vec3 lightDir = vec3(10.0, 10.0, -1.0);

void main() {
    vec3 invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;
    vec3 invEye = normalize(invMatrix * vec4(vPosition - eyePosition, 0.0)).xyz;
    vec3 halfLE = normalize(invLight + invEye);
    float diffuse = clamp(abs(dot(vNormal, invLight)), 0.0, 1.0);
    float specular = pow(clamp(dot(vNormal,halfLE),0.0,1.0),150.0);

    vec3 I = normalize(eyePosition - vPosition);
    vec3 R = reflect(I, normalize(vNormal)); // 反射
    vec3 envColor = texture(cubeTexture, R).rgb;

    vec4 ambientColor = vec4(envColor*0.7, 1.0);
    vec4 destColor = vec4(envColor, 1.0) * vec4(vec3(diffuse),1.0)+vec4(vec3(specular), 1.0)+ambientColor;
    fragColor = destColor;
    // fragColor = vColor;
}
