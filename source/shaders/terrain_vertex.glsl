#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Color;
out vec2 TexCoords;
void main()
{
    gl_Position = projection * view * model * vec4(Position, 1.0);
    TexCoords = aTexCoords;   
    Color = vec4(Position.y / 10.0);
}
