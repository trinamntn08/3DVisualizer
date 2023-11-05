#version 330 core
in vec3 texCubeCoords;

out vec4 fragColor;

uniform samplerCube u_texture_skybox;

void main() {
    fragColor = texture(u_texture_skybox, texCubeCoords);
}