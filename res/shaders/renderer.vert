#version 330

// Input arrays
layout (location = 0) in vec3 world;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

// Transformation matrices
uniform mat4 w;
uniform mat4 vp;
uniform mat4 sunVP;

// Interpolated values output
out vec2 interTex;
out vec3 interNorm;
out vec4 interSunPos;

void main() {
    // Outputting the object fragment position
    gl_Position = vp * w * vec4(world, 1);

    // Outputting the sun fragment position
    interSunPos = sunVP * w * vec4(world, 1);

    // Outputting the interpolated texture coords
    interTex = tex;

    // Outputting the interpolated normal
    interNorm = (w * vec4(norm, 0)).xyz;
}
