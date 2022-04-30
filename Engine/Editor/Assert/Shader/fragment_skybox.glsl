#version 330 core
out vec4 color;

in vec3 TexCoords;

uniform samplerCube environmentMap;
uniform float exposure;

void main()
{
    const float gamma = 2.2;

    vec3 envColor = texture(environmentMap, TexCoords).rgb;
    vec3 mapped = vec3(1.0) - exp(-envColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
}