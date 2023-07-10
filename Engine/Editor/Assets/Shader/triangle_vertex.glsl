#version 460 core

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

vec2 positions[3] = vec2[](
    vec2(0.5, -0.5),
    vec2(-0.5, -0.5),
    vec2(0.0, 0.5)
);

void main() {
    gl_Position = u_ViewProjection * u_Transform * vec4(positions[gl_VertexID], 0.0, 1.0);
}