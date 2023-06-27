#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

uniform vec3 u_ViewPos;
uniform vec3 u_Color;

void main() {
    vec3 lightColor = vec3(0.6f, 0.6f, 0.6f);
    vec3 ambient = 0.7 * lightColor;
    
    vec3 lightDir = vec3(1.0f, 1.0f, 1.0f);
    float diff = max(dot(v_Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, v_Normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 15);
    vec3 specular = 0.3 * spec * lightColor;

    vec3 result = ambient + diffuse + specular;

    vec3 albedo = vec3(0.7f, 0.7f, 0.7f);

    color = vec4(result * albedo, 1.0f);
}