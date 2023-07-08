#version 460 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D g_Color;
uniform float exposure;

vec3 acesFilm(const vec3 hdr_color, const float exposure) {
    const float A = 2.51f;
    const float B = 0.03f;
    const float C = 2.43f;
    const float D = 0.59f;
    const float E = 0.14f;

    vec3 ldr_color = exposure * hdr_color;
    ldr_color = (ldr_color * (A* ldr_color + B)) / (ldr_color * (C* ldr_color + D) + E);

    return ldr_color;
}

void main()
{
    vec3 color = texture(g_Color, v_TexCoord).rgb;
    vec3 mapped = acesFilm(color, exposure);

    FragColor = vec4(mapped, 1.0);
}