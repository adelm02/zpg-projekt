#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 fragColor;
uniform mat4 model;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos; //vypocet smeru svetla
out vec3 Normal;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    fragColor = vec3(0.385, 0.647, 0.812);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}

