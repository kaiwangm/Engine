#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
    vec3 worldPosition = (u_Transform * vec4(a_Position, 1.0)).xyz;
    vec3 worldNormal = (u_Transform * vec4(a_Normal, 0.0)).xyz;

    v_Position = worldPosition;
    v_Normal = worldNormal;
    v_TexCoord = a_TexCoord;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}