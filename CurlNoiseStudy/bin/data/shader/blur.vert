#version 150
precision mediump float;

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
in vec4 color;
in vec4 normal;

out vec2 vTexCoord;

void main() {
  gl_Position = modelViewProjectionMatrix * position;
  vTexCoord = texcoord;
}
