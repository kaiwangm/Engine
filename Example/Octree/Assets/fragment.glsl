#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;

uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    if (texColor.a < 0.1) discard;
    color = texColor;
}