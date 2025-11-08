#version 330 core

in vec3 fragColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform bool useTexture;

void main()
{
    vec3 color;

    if (useTexture) {
        color = texture(diffuseTexture, TexCoord).rgb;
    } else {
        color = fragColor;
    }

    FragColor = vec4(color, 1.0);
}