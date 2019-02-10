#version 150
precision mediump float;

uniform sampler2DRect posAndAgeTex;
uniform mat4 modelViewProjectionMatrix;

in vec2 texcoord;
in vec4 position;
in vec4 color;
in vec4 normal;

out vec2 vTexCoord;
out vec4 vColor;
out vec4 vNormal;

void main() {
  vec3 pos = texture(posAndAgeTex, texcoord).xyz;

  vColor = color;
  vTexCoord = normalize(texcoord);
  vNormal = normal;

  gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
}
