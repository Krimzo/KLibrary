#version 330

layout (location = 0) in vec3 world;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 color;

uniform mat4 transform;

out vec3 interColor;

void main() {
    gl_Position = transform * vec4(world, 1);
    interColor = color;
}
