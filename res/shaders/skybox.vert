#version 330

// Input arrays
layout (location = 0) in vec3 world;

// Transformation matrices
uniform mat4 vp;

// Interpolated values output
out vec3 interTex;

void main() {
    // Outputting the transformed world coords
    gl_Position = (vp * vec4(world, 0)).xyww;
    
    // Outputting the interpolated texture coords
    interTex = world;
}
