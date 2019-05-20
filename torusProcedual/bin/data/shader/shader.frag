#version 150
precision highp float;

uniform mat4 invMatrix;

in vec3 vNormal;
in vec4 vColor;

out vec4 fragColor;

const vec3 lightDir = vec3(10.0, 1.0, 5.0);

void main() {
	vec3 ambient = vColor.rgb * 0.3;
	vec3 invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;
	float diffuse = abs(dot(invLight, normalize(vNormal)));
	float specular = pow(diffuse, 20.0);
	vec4 destColor = vec4(ambient, 1.0) + diffuse * vColor * 0.7 + vec4(vec3(specular), 1.0);

	fragColor = destColor;
}
