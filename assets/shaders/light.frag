#version 330 core

out vec4 FragColor;

uniform vec4 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float ambientStrength;

in vec3 vWorldPos;
in vec3 vNormal;

void main() {
    FragColor = vec4(lightColor, 1.0);
}