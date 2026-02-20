#version 330 core

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D diffuseMap;
uniform bool hasTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float ambientStrength;

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 TexCoords;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vWorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor * lightIntensity;
    vec3 albedo;
    if (hasTexture) {
        albedo = texture(diffuseMap, TexCoords).rgb;
    } else {
        albedo = color.rgb;
    }
    vec3 litColor = (ambient + diffuse) * albedo;
    FragColor = vec4(litColor, color.a);
}
