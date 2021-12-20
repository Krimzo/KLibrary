#version 330

layout (location = 0) in vec3 xyz;
layout (location = 1) in vec2 uv;

uniform mat4 wvp;

out vec2 interUV;

void main() {
    gl_Position = wvp * vec4(xyz, 1);
    interUV = uv;
}
