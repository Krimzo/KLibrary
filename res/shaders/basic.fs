#version 330

uniform sampler2D texture0;

out vec4 pixelColor;

in vec2 interUV;
in vec3 interRGB;

void main () {
    pixelColor = mix(texture2D(texture0, interUV), vec4(interRGB, 1), 0);
}
