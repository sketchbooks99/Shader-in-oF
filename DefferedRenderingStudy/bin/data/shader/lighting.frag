#version 330 core 
precision mediump float;

struct Light {
    vec3 position;
    vec3 color;
};

const int LIGHT_NUM = 20;

in vec2 vTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform mat4 model;
uniform int debugMode;
uniform int index;
uniform float lightDistance;
uniform Light light[LIGHT_NUM];
uniform vec3 viewPos;
uniform float lightAttenuation;
uniform int arrayDebug;
uniform float disCoef;

out vec4 fragColor;

void main() {
    vec3 fragPos = texture(gPosition, vTexCoord).rgb;
    vec3 Normal = texture(gNormal, vTexCoord).rgb;
    vec3 Color = texture(gColorSpec, vTexCoord).rgb;
    float isCalc = texture(gColorSpec, vTexCoord).a;

    vec3 lighting = Color.rgb * .3;

    if(isCalc > 0.5) {
        // array debug
        if(arrayDebug == 1) {
            vec3 lightPos = light[index].position;
            vec3 lightDir = normalize(lightPos - fragPos);
            float lightLen = length(lightDir) * disCoef;
            float attenuation = 1.0 / (lightAttenuation * lightLen * lightLen);
            lightDir = normalize(lightDir);
            float diffuse = max(0.0, dot(Normal, lightDir));
            vec3 diffuseColor = max(0.0, dot(Normal, lightDir)) * light[index].color;
            vec3 specularColor = vec3(pow(diffuse, 20.0)) * light[index].color;
            lighting += (diffuseColor + specularColor) * attenuation;
        } else {
            for(int i=0; i<LIGHT_NUM; i++) {
                vec3 lightPos = (vec4(light[i].position, 1.0)).xyz;
                vec3 lightDir = (lightPos - fragPos);
                float lightLen = length(lightDir) * disCoef;
                float attenuation = 1.0 / (lightAttenuation * lightLen * lightLen);
                lightDir = normalize(lightDir);
                float diffuse = max(0.0, dot(Normal, lightDir));
                vec3 diffuseColor = max(0.0, dot(Normal, lightDir)) * light[i].color;
                vec3 specularColor = vec3(pow(diffuse, 20.0)) * light[i].color;
                lighting += (diffuseColor + specularColor) * attenuation;
            }
        }
    } else {
        lighting = Color.rgb;
    }
    // lighting = max(dot(Normal, viewPos), 0.0) * vec3(1.0);

    if(debugMode == 0) {
        fragColor = vec4(lighting, 1.0);
    }
    else if(debugMode == 1) {
        fragColor = vec4(fragPos, 1.0);
    }
    else if(debugMode == 2) {
        fragColor = vec4(Normal, 1.0);
    }
    else if(debugMode == 3) {
        fragColor = vec4(Color, 1.0);
    }
}