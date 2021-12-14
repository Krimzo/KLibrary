#version 330

layout (location = 0) in vec3 xyz;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 rgb;

uniform mat4 wvp;

out vec2 interTex;
out vec3 interColor;

void main() {
    gl_Position = wvp * vec4(xyz, 1);
    interTex = uv;
    interColor = rgb;
}
