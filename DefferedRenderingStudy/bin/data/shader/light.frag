#version 330 core

layout(location = 0) out vec4 gRender;

in vec3 vPosition;
in vec3 vNormal;
const vec3 lightPos = vec3(0.0,0.0,0.0);

uniform vec3 lightColor;
void main() {
    vec3 lightDir = normalize(lightPos - vPosition);
    vec3 ambient = lightColor * .3;
    float diffuse = max(0.3, dot(vNormal, lightDir));
    float specular = pow(diffuse, 20.0);
    vec3 destColor = ambient + diffuse * lightColor + vec3(specular);
    gRender = vec4(destColor, 1.0);
}