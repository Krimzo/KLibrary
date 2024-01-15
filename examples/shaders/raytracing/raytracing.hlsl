struct RayPayload
{
    float4 color;
};

cbuffer GLOBAL_CB : register(b0)
{
    float4x4 INVERSE_CAMERA;
    float3 CAMERA_ORIGIN;
};

RaytracingAccelerationStructure SCENE : register(t0, space0);
RWTexture2D<float4> OUTPUT_TEXTURE : register(u0);

// Ray generation shader
[shader("raygeneration")]
void ray_gen_shader()
{
    const float2 screen_coords = (float2) DispatchRaysIndex() / (float2) DispatchRaysDimensions();
    const float2 ndc = screen_coords * 2.0f - 1.0f;
    float4 ray_direction = mul(float4(ndc, 1.0f, 1.0f), INVERSE_CAMERA);
    ray_direction /= ray_direction.w;

    RayDesc ray;
    ray.Origin = CAMERA_ORIGIN;
    ray.Direction = ray_direction.xyz;
    ray.TMin = 0.01f;
    ray.TMax = 50.0f;
    
    RayPayload payload = { float4(0.0f, 0.0f, 0.0f, 1.0f) };
    TraceRay(SCENE, RAY_FLAG_NONE, ~0, 0, 1, 0, ray, payload);
    OUTPUT_TEXTURE[DispatchRaysIndex().xy] = payload.color;
}

// Closest hit shader
[shader("closesthit")]
void closest_hit_shader(inout RayPayload payload, in BuiltInTriangleIntersectionAttributes _)
{
    payload.color = float4(1.0f, 0.75f, 0.35f, 1.0f);
}

// Miss shader
[shader("miss")]
void miss_shader(inout RayPayload payload)
{
    payload.color = float4(0.1f, 0.1f, 0.1f, 1.0f);
}
