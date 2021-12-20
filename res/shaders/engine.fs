#version 330

uniform sampler2D texture0;
uniform vec3 ambient;

in vec2 interUV;

out vec4 pixelColor;

void main () {
    pixelColor = texture2D(texture0, interUV) * vec4(ambient, 1);
}
