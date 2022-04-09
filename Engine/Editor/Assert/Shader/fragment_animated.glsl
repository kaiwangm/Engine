#version 330 core

layout(location = 0) out vec4 out_colour;

in vec2 pass_textureCoords;
in vec3 pass_normal;

uniform sampler2D diffuseMap;

void main(void) {
    vec4 diffuseColour = texture(diffuseMap, pass_textureCoords);
    out_colour = diffuseColour;
}