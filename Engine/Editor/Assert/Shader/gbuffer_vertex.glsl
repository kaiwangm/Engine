#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_MProjection;
uniform mat4 u_MView;
uniform mat4 u_MTransform;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
    vec4 vPosition = u_MView * u_MTransform * vec4(a_Position, 1.0);
    v_Position = vPosition.xyz;
    gl_Position = u_MProjection * vPosition;
    
    v_TexCoord = a_TexCoord;

    mat3 normalMatrix = transpose(inverse(mat3(u_MView * u_MTransform)));
    v_Normal = normalMatrix * a_Normal;
}