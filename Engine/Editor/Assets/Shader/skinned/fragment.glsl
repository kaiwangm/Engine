#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec3 v_Normal;

void main()
{
    color = vec4(v_Normal, 1.0);
}