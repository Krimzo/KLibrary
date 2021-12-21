#version 330

// Skybox texture
uniform samplerCube skybox;

// Interpolated values input
in vec3 interTex;

// Pixel color output
out vec4 pixelColor;

void main () {
    // Setting the pixel color
    pixelColor = texture(skybox, interTex);
}
