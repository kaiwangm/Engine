#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;

uniform sampler2D u_Texture;

void main() {
    vec4 tex_color = texture(u_Texture, v_TexCoord);
    color = vec4(v_TexCoord, 1.0f, tex_color.a);
}