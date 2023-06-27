#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform float u_BoneLength;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPos;

void main() {
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    v_TexCoord = a_TexCoord;
    vec4 position = u_Transform * vec4(u_BoneLength * a_Position, 1.0f);
    v_FragPos = vec3(position);
    gl_Position = u_ViewProjection * position;
}