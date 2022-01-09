#version 330

// Info
uniform vec2 frameSize;
uniform float zoom;
uniform vec2 pos;

// Pixel color output
out vec4 pixelColor;

// Computes a complex number power
vec2 complexPow(vec2 a, int n) {
    for(int i = 1; i < n; i++) {
        a = vec2(a.x * a.x - a.y * a.y, 2 * a.x * a.y);
    }
    return a;
}

void main() {
    // Setting the default color
    pixelColor = vec4(1);

    // Getting the uv coordinates
    vec2 uv = ((gl_FragCoord.xy / frameSize) * 2 - 1) * vec2(frameSize.x / frameSize.y, 1);

    // Setting the zoom
    uv /= zoom;

    // Setting the position
    uv += pos;

    // Calculating the reaction
    vec2 num = uv;
    for(int i = 0; i < 256; i++) {
        // Checking the distance
        if(length(num) > 2) {
            pixelColor = vec4(vec3(i / 64.0), 1);
            break;
        }

        // Computing the next point
        num = complexPow(num, 2) + uv;
    }
}
