#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;

void main() {
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}