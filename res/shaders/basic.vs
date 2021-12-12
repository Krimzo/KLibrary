#version 330

layout (location = 0) in vec3 world;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 color;

uniform float scaleFactor;

out vec3 interColor;

void main() {
    gl_Position = vec4(world.x * scaleFactor, world.y * scaleFactor, world.z, 1);
    interColor = color;
}
