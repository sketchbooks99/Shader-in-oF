#version 150
precision mediump float;

#pragma include "util.frag"
#pragma include "noise4D.frag"

uniform sampler2DRect posAndAgeTex;
uniform sampler2DRect velAndMaxTex;
uniform float time;
uniform float timestep;
uniform float scale;
uniform float radius;
uniform vec3 emitterPos;
uniform vec3 prevEmitterPos;
uniform int mode;

in vec2 vTexCoord;

out vec4 vFragData0;
out vec4 vFragData1;

void main() {
  vec4 posAndAge = texture(posAndAgeTex, vTexCoord);
  vec4 velAndMax = texture(velAndMaxTex, vTexCoord);

  float mode = floor(mod(time, 5.0));

  vec3 pos = posAndAge.xyz; // 前の位置
  vec3 vel = velAndMax.xyz; // 前の速度

  float age = posAndAge.w; // 経過時間
  float maxAge = velAndMax.w; // 生存期間

  age++;

  // パーティクルの生存期間が切れたら初期化
  if(age >= maxAge) {
    age = 0;
    maxAge = 60.0 + random(pos.xx)*60.0;
    float mode = floor(mod(time+pos.x, 2.0));
    float posR = 200.0;
    if(mode == 1.0) posR = radius;
    // maxAge = 60.0 + random(pos.xx) * 60.0;
    float theta = 2.0 * PI * random(pos.yy);
    float phi = PI * random(pos.zz);
    float r = 5.0 * random(pos.xy);
    vec3 startPos;
    // if(mode == 0.0) {
    //   startPos = vec3(clamp(pos.x, -200.0, 200.0), clamp(pos.y, -200.0, 200.0), 0.0);
    // } else if(mode == 1.0) {
    //   float radius = 200.0;
    //   startPos = vec3(sin(vTexCoord.x)*radius, cos(vTexCoord.x)*radius, random(pos.zx)*30.0);
    // } else if(mode==2.0) {
    //   startPos = vec3(sin(vTexCoord.x)*50.0, sin(vTexCoord.y)*600.0, 0.0);
    // } else if(mode==3.0) {
    //   startPos = vec3(sin(vTexCoord.x)*cos(vTexCoord.y)*200.0, sin(vTexCoord.x)*sin(vTexCoord.y)*200.0, cos(vTexCoord.x));
    // } else if(mode==4.0) {
    //   startPos = vec3(sin(vTexCoord.x)*800.0, sin(vTexCoord.y)*50.0, random(pos.zx)*30.0);
    // }
    startPos = vec3(sin(vTexCoord.x)*cos(vTexCoord.y)*posR, sin(vTexCoord.x)*sin(vTexCoord.y)*posR, cos(vTexCoord.x)*posR);
    pos = startPos + vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
    vel.xyz = normalize(startPos);
  }

  // Curl Noiseで速度を更新
  vel.x += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 0.1352 * time * timestep));
  vel.y += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 1.2814 * time * timestep));
  vel.z += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 2.5564 * time * timestep));

  pos += vel;

  vFragData0.rgba = vec4(pos, age);
  vFragData1.rgba = vec4(vel, maxAge);
}
