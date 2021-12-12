#version 330

out vec4 pixelColor;

in vec3 interColor;

void main () {
    pixelColor = vec4(interColor, 1);
}
