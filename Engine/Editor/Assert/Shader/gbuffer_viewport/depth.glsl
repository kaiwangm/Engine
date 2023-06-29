#version 460 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D g_Depth;

void main()
{    
    float depth = texture(g_Depth, v_TexCoord).r;
    FragColor = vec4(vec3(depth), 1.0);
}  