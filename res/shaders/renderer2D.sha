#shader vertex
#version 330

// Input arrays
layout (location = 0) in vec2 world;
layout (location = 1) in vec2 tex;

// Transformation matrices
uniform mat3 w;

// Interpolated values output
out vec2 interTex;

void main() {
    // Outputting the object fragment position
    gl_Position = vec4(w * vec3(world, 0.5), 1);

    // Outputting the interpolated texture coords
    interTex = tex;
}


#shader fragment
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
