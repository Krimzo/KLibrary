#shader vertex
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

    // Outputting the interpolated texture coords
    interTex = tex;

    // Outputting the interpolated normal
    interNorm = (w * vec4(norm, 0)).xyz;

    // Outputting the sun fragment position
    interSunPos = sunVP * w * vec4(world, 1);
}


#shader fragment
#version 330

// Object texture
uniform sampler2D texture0;
uniform sampler2D shadowMap;

// Ambient light
uniform vec3 dark;

// Directional light
uniform vec3 sunL;
uniform vec3 sunD;

// Interpolated values input
in vec2 interTex;
in vec3 interNorm;
in vec4 interSunPos;

// Pixel color output
out vec4 pixelColor;

float computeShadow(float dotLN) {
    // Shadow value
	float shadow = 0;

	// Transforming the light coords to cull space
	vec3 lightCoords = interSunPos.xyz / interSunPos.w;

	// Checking if the pos is less than 1
	if(lightCoords.z <= 1) {
		// Transforming from range [-1, 1] to [0, 1]
		lightCoords = lightCoords * 0.5 + 0.5;

		// Prevents shadow acne
		float bias = 0.0025;
		float biasMin = 0.0005;
		bias = max(bias * (1 - dotLN), biasMin);

		// Smoothing the shadow
		int smoothRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -smoothRadius; y <= smoothRadius; y++) {
		    for(int x = -smoothRadius; x <= smoothRadius; x++) {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r + bias;
				if (lightCoords.z > closestDepth) {
					shadow++;
				}
		    }
		}

		// Calculating the average shadow
		shadow /= pow((smoothRadius * 2 + 1), 2);
	}

	// Returning the shadow value
	return (1 - shadow);
}

void main () {
    // Texture color
    vec3 textureColor = texture(texture0, interTex).rgb;

    // Ambient light color
    vec3 ambientColor = dark;

    // Directional light color
    vec3 directColor = vec3(0);

    // Calculating the directional light intensity
    float diffuseFactor = dot(-sunD, normalize(interNorm));

    // Checking the diffuse factor
    if (diffuseFactor > 0) {
        directColor = sunL * diffuseFactor;
    }

	// Computing the shadow factor
	float shadowFac = computeShadow(diffuseFactor);

    // Setting the pixel color
    pixelColor = vec4(textureColor * (shadowFac * directColor + ambientColor), 1);
}
