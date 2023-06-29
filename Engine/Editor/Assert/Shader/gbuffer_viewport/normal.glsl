#version 460 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D g_Normal;

void main()
{    
    FragColor = vec4(texture(g_Normal, v_TexCoord).xyz, 1.0);
}  