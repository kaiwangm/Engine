#version 330 core

layout (location = 0) out vec3 g_Position;
layout (location = 1) out vec3 g_Normal;
layout (location = 2) out vec4 g_Albedo;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform bool useAlbedoMap = false;
uniform vec3 in_albedo;
uniform sampler2D albedoMap;

void main()
{    
    g_Position = v_Position;
    g_Normal = normalize(v_Normal);
    
    g_Albedo = vec4(1.0);
    if (useAlbedoMap)
    {
        g_Albedo = vec4(texture(albedoMap, v_TexCoord).rgb, 1.0);
    }
    else
    {
        g_Albedo = vec4(in_albedo, 1.0);
    }
}  