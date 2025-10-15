#version 330 core

in vec3 fragColor;
out vec4 frag_color;
uniform vec3 viewPos;
void main() {
    frag_color = vec4(fragColor, 1.0);
}