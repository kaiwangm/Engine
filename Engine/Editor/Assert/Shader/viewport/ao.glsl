#version 460 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D g_AO;

float near = 0.1;
float far  = 800.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{   
    float ao = texture(g_AO, v_TexCoord).r;
    FragColor = vec4(vec3(LinearizeDepth(ao)), 1.0);
}  