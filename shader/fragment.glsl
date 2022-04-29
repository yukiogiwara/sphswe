#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    FragColor = vec4(diff*vec3(1.0f, 0.5f, 0.2f), 1.0f);
}