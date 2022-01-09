#version 330


// Pixel color output
out vec4 pixelColor;

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(1600, 900);

    if (uv.x > 0.5) {
        pixelColor = vec4(1, 1, 1, 1);
    }
    else {
        pixelColor = vec4(0.1, 0.1, 0.1, 1);
    }
}
