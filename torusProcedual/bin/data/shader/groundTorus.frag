#version 150
precision highp float;

uniform mat4 invMatrix;

in vec4 vColor;
in vec3 vNormal;

out vec4 fragColor;

const vec3 lightDir = vec3(1.0, 0.1, 0.1);

//void main() {
//	vec4 ambient = vColor * 0.2;
//	vec3 invLight = normalize(invMatrix * vec4(lightDir, 1.0)).xyz;
//	float diffuse = dot(invLight, vNormal);
//	float specular = pow(diffuse, 10.0);
//	vec4 destColor = ambient + vec4(vColor.xyz * diffuse, 1.0) + vec4(specular);
//
//	fragColor = destColor;
//}

void main() {
	fragColor = vColor;
}