// https://github.com/Baksonator/ssao/blob/master/shaders/ssao-frag.glsl
#version 460 core

layout (location = 0) out float ambientOcclusion;

in vec2 v_TexCoord;

uniform sampler2D g_Position;  // gPosition
uniform sampler2D g_Normal;  // gNormal
uniform sampler2D g_Depth;  // texNoise

int kernelSize = 256;
uniform vec3 samples[256];

uniform float radius;
uniform float bias;
uniform float power;

uniform int screenWidth;
uniform int screenHeight;

vec2 noiseScale = vec2(screenWidth/4.0, screenHeight/4.0);

uniform mat4 projection;

void main()
{
    vec3 fragPos = texture(g_Position, v_TexCoord).xyz;
    vec3 normal = texture(g_Normal, v_TexCoord).rgb;
    vec3 randomVec = texture(g_Depth, v_TexCoord * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;

	for(int i = 0; i < kernelSize; i += 1)
	{
		vec3 sample_pos = samples[i];
		sample_pos = TBN * sample_pos;
		sample_pos = fragPos + sample_pos * radius;

		vec4 offset = vec4(sample_pos, 1.0);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz  = offset.xyz * 0.5 + 0.5;

		float sampleDepth = texture(g_Position, offset.xy).z;

		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));

		occlusion += (sampleDepth >= sample_pos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}
    occlusion = 1.0 - (occlusion / kernelSize);
	occlusion = pow(occlusion, power);
    
    ambientOcclusion = occlusion;
}