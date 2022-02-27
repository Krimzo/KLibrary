// Vertex shader
struct vOut {
    float4 screen : SV_POSITION;
};

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    data.screen = float4(pos, 1.0f);

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 frameSize;
    float4 camPos;
    float4 objCol;
    float4 vertCount;
    matrix ivp;
    matrix w;
}

SamplerState samp : register(s0);
Texture2D vertexData : register(t0);

struct Ray {
    float3 origin;
    float3 direction;

    // Intersection with a triangle
    bool intersect(float3 A, float3 B, float3 C, out float3 interPoint) {
        // Const epsilon
        const float EPSILON = 1e-7f;

        // Calculating triangle edges
        const float3 edge1 = B - A;
        const float3 edge2 = C - A;

        // More calculations
        const float3 h = cross(direction, edge2);
        const float a = dot(edge1, h);

        // Parallel check
        if (a > -EPSILON && a < EPSILON) {
            return false;
        }

        // More calculations
        const float3 s = origin - A;
        const float f = 1.0f / a;
        const float u = f * dot(s, h);

        // More checks
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        // More calculations
        const float3 q = cross(s, edge1);
        const float v = f * dot(direction, q);

        // More checks
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        // More calculations that I don't understand
        const float t = f * dot(edge2, q);

        // Ray intersection
        if (t > EPSILON) {
            interPoint = origin + direction * t;
            return true;
        }
        // Line intersection but no ray intersection
        else {
            return false;
        }
    }
};

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel;

    // Pixel can be cleared
	bool canClear = true;

	// Pixel depth
    float pixelDepth = 1e+300 * 1e+300;

    // Ndc calc
    float2 ndc = float2(data.screen.x / frameSize.x, (frameSize.y - data.screen.y) / frameSize.y);
    ndc *= 2.0f;
    ndc -= 1.0f;

    // Ray direction calc
    float4 rayDir = mul(float4(ndc, 1.0f, 1.0f), ivp);
    rayDir /= rayDir.w;

    // Ray creation
    Ray ray = { camPos.xyz, rayDir.xyz };

    // Loop through triangles
    const int trCount = int(vertCount.x) / 3;
    const float vStep = 1.0f / trCount;
    for (int i = 0; i < trCount; i++) {
        // Temp triangle points
        float4 trA = float4(vertexData.Sample(samp, float2(0.25f, i * vStep)).xyz, 1.0f);
        float4 trB = float4(vertexData.Sample(samp, float2(0.50f, i * vStep)).xyz, 1.0f);
        float4 trC = float4(vertexData.Sample(samp, float2(0.75f, i * vStep)).xyz, 1.0f);
        
        // World matrix multiplication
        trA = mul(trA, w);
        trB = mul(trB, w);
        trC = mul(trC, w);

        // Perspective divide
        trA /= trA.w;
        trB /= trB.w;
        trC /= trC.w;

        // Intersection test
        float3 intersection;
        if (ray.intersect(trA.xyz, trB.xyz, trC.xyz, intersection)) {
	        // Clear switch
            canClear = false;

	        // Depth calc
            const float tempDepth = length(intersection - camPos.xyz);

	        // Depth check
            if (tempDepth < pixelDepth) {
		        // Depth save
                pixelDepth = tempDepth;

		        // Pixel draw
                pixel = objCol;
            }
        }
        else if (canClear) {
	        // Background draw
            pixel = float4(0.2f, 0.2f, 0.2f, 1.0f);
        }
    }

    return pixel;
}
