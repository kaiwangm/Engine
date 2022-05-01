#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

void main() {
    TexCoords = a_TexCoord;
    WorldPos = vec3(u_Transform * vec4(a_Position, 1.0));
    Normal = mat3(u_Transform) * a_Normal;

    gl_Position = u_ViewProjection * vec4(WorldPos, 1.0f);
}