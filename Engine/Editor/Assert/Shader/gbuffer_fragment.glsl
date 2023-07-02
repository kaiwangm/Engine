#version 460 core

layout (location = 0) out vec3 g_Position;
layout (location = 1) out vec3 g_Normal;
layout (location = 2) out vec4 g_Albedo;
layout (location = 3) out float g_Opacity;
layout (location = 4) out float g_Depth;
layout (location = 5) out float g_Roughness;
layout (location = 6) out float g_Metallic;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform bool useAlbedoMap = false;
uniform vec3 in_albedo;
uniform sampler2D albedoMap;

uniform bool useNormalMap = false;
uniform sampler2D normalMap;

uniform bool useRoughnessMap = false;
uniform float in_roughness;
uniform sampler2D roughnessMap;

uniform bool useMetallicMap = false;
uniform float in_metallic;
uniform sampler2D metallicMap;

uniform bool useAOMap = false;
uniform float in_ao;
uniform sampler2D aoMap;

float near = 0.1; 
float far  = 800.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

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

    g_Opacity = 1.0;

    g_Depth = LinearizeDepth(gl_FragCoord.z);

    g_Roughness = in_roughness;
    if (useRoughnessMap)
    {
        g_Roughness = texture(roughnessMap, v_TexCoord).r;
    }

    g_Metallic = in_metallic;
    if (useMetallicMap)
    {
        g_Metallic = texture(metallicMap, v_TexCoord).r;
    }
}  