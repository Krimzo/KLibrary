#version 330

// Input arrays
layout (location = 0) in vec3 world;

void main() {
    gl_Position = vec4(world, 1);
}
