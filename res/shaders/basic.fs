#version 330

uniform sampler2D texture0;

out vec4 pixelColor;

in vec2 interTex;
in vec3 interColor;

void main () {
    pixelColor = mix(texture2D(texture0, interTex), vec4(interColor, 1), 0.5);
}
