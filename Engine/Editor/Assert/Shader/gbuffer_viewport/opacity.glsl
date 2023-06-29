#version 460 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D g_Opacity;

void main()
{    
    float opacity = texture(g_Opacity, v_TexCoord).x;
    FragColor = vec4(opacity, opacity, opacity, 1.0);
}  