#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;

out vec3 fragColor;

uniform mat4 model;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    fragColor = vn;
    gl_Position = projectionMatrix * viewMatrix * model * vec4(vp, 1.0);
}