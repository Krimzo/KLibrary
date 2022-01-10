#version 330

// Info
uniform vec2 frameSize;
uniform float zoom;
uniform vec2 pos;
uniform float startPos;

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
    // Getting the uv coordinates
    vec2 uv = ((gl_FragCoord.xy / frameSize) * 2 - 1) * vec2(frameSize.x / frameSize.y, 1);

    // Setting the zoom
    uv /= zoom;

    // Setting the position
    uv += pos;

    // Setting the default color
    pixelColor = vec4(1);

    // Calculating the reaction
    float n = 64;
    vec2 num = startPos + uv;
    for(int i = 0; i < n; i++) {
        // Checking the distance
        if(length(num) > 2) {
            pixelColor *= i / n;
            break;
        }

        // Computing the next point
        num = complexPow(num, 2) + uv;
    }
}
