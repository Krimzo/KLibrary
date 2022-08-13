// Vertex shader
float4 vShader(float3 pos : KL_Position) : SV_Position {
    return float4(pos, 1.0f);
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 frameSize;
    matrix inverseCamera;
    float4 cameraPosition;
    float4 sunDirection;
}

float4 pShader(float4 screen : SV_Position) : SV_Target {
    float3 pixel = { 0, 0, 0 };
    
    float2 ndc = float2(screen.x, frameSize.y - screen.y) / float2(frameSize.x, frameSize.y);
    ndc = ndc * 2.0f - 1.0f;
	
    float4 ray = mul(float4(ndc, 1.0f, 1.0f), inverseCamera);
    ray /= ray.w;
    
    float3 rayDir = normalize(ray.xyz);
    
    const float3 skyTopColor = { 0.62f, 0.77f, 0.88f };
    const float3 skyBottomColor = { 0.89f, 0.93f, 0.96f };
    const float3 sunSkyColor = { 0.98f, 0.9f, 0.76f };
	
    const float2 sunRadiuses = { 0.75f, 1.55f };

    const float skyMixValue = (dot(-rayDir, float3(0.0f, 1.0f, 0.0f)) + 1.0f) * 0.5f;
    pixel = lerp(skyTopColor, skyBottomColor, skyMixValue);
    pixel *= dot(-sunDirection.xyz, float3(0.0f, 1.0f, 0.0f));
		
    const float sunAngle = degrees(acos(dot(rayDir, -sunDirection.xyz)));
    pixel = lerp(sunSkyColor, pixel, smoothstep(sunRadiuses.x, sunRadiuses.y, sunAngle));
    
    return float4(pixel, 1.0f);
}
