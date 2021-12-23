#version 330

// Object texture
uniform sampler2D texture0;

// Ambient light
uniform vec3 dark;

// Directional light
uniform vec3 sunL;
uniform vec3 sunD;

// Interpolated values input
in vec2 interTex;
in vec3 interNorm;

// Pixel color output
out vec4 pixelColor;

void main () {
    // Texture color
    vec4 textureColor = texture2D(texture0, interTex);

    // Ambient light color
    vec4 ambientColor = vec4(dark, 1);

    // Directional light color
    vec4 directColor = vec4(0, 0, 0, 1);

    // Calculating the directional light intensity
    float diffuseFactor = dot(normalize(interNorm), -sunD);

    // Checking the diffuse factor
    if (diffuseFactor > 0) {
        directColor = vec4(sunL, 1) * diffuseFactor;
    }

    // Setting the pixel color
    pixelColor = textureColor * (directColor + ambientColor);
}
