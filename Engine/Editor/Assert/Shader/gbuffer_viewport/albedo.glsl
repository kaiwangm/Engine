#version 460 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D g_Albedo;

void main()
{    
    FragColor = vec4(texture(g_Albedo, v_TexCoord).xyz, 1.0);
}  