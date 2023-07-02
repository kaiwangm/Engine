#version 460 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D g_AO;

void main()
{   
    float ao = texture(g_AO, v_TexCoord).r;
    FragColor = vec4(vec3(ao), 1.0);
}  