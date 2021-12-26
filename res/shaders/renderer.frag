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

float computeShadow() {
    // Shadow value
	float shadow = 0;

	// Transforming the light coords to cull space
	vec3 lightCoords = interSunPos.xyz / interSunPos.w;

	// Checking if the pos is less than 1
	if(lightCoords.z <= 1) {
		// Transforming from range [-1, 1] to [0, 1]
		lightCoords = lightCoords * 0.5 + 0.5;

		// Prevents shadow acne
		float bias = max(0.025 * (1 - dot(normalize(interNorm), -sunD)), 0.0005);

		// Smoothing the shadow
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++) {
		    for(int x = -sampleRadius; x <= sampleRadius; x++) {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (lightCoords.z > closestDepth + bias) {
					shadow++;
				}
		    }
		}

		// Returning the average shadow
		return shadow / pow((sampleRadius * 2 + 1), 2);
	}
}

void main () {
    // Texture color
    vec3 textureColor = texture(texture0, interTex).rgb;

    // Ambient light color
    vec3 ambientColor = dark;

    // Directional light color
    vec3 directColor = vec3(0);

    // Calculating the directional light intensity
    float diffuseFactor = dot(normalize(interNorm), -sunD);

    // Checking the diffuse factor
    if (diffuseFactor > 0) {
        directColor = sunL * diffuseFactor;
    }

	// Computing the shadow factor
	float shadowFac = computeShadow();

    // Setting the pixel color
    pixelColor = vec4(textureColor * ((1 - shadowFac) * directColor + ambientColor), 1);
}
