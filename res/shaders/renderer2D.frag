#version 330

// Object texture
uniform sampler2D texture0;

// Interpolated values input
in vec2 interTex;

// Pixel color output
out vec4 pixelColor;

void main () {
    // Texture color
    vec4 textureColor = texture(texture0, interTex);

    // Setting the pixel color
    pixelColor = textureColor;
}
