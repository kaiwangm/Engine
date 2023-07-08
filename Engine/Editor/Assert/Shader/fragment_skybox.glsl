#version 330 core
out vec4 color;

in vec3 TexCoords;

uniform samplerCube environmentMap;

uniform float mipLevel = 0.0f;

void main()
{
    vec3 envColor = texture(environmentMap, TexCoords, mipLevel).rgb;

    color = vec4(envColor, 1.0);
}