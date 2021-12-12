#version 330

layout (location = 0) in vec3 world;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 color;

out vec3 interColor;

void main() {
    gl_Position = vec4(world, 1);
    interColor = color;
}
