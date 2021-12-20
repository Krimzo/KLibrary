#version 330

uniform sampler2D texture0;

in vec2 interUV;

out vec4 pixelColor;

void main () {
    pixelColor = texture2D(texture0, interUV);
}
