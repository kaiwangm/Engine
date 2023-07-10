#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
    vec4 position = u_Transform * vec4(a_Position, 1.0f);
    gl_Position = u_ViewProjection * position;
}