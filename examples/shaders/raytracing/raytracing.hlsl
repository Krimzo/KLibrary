struct Payload
{
    float3 color;
    float missed;
};

RaytracingAccelerationStructure SCENE : register(t0);
RWTexture2D<float4> RENDER_TARGET : register(u0);

static const float3 CAMERA = float3(0.0f, 1.5f, -7.0f);
static const float3 SUN = float3(0.0f, 200.0f, 0.0f);
static const float3 SKY_TOP = float3(0.24f, 0.44f, 0.72f);
static const float3 SKY_BOTTOM = float3(0.75f, 0.86f, 0.93f);

void hit_cube(inout Payload payload, float2 uv);
void hit_mirror(inout Payload payload, float2 uv);
void hit_floor(inout Payload payload, float2 uv);

[shader("raygeneration")]
void ray_generation_shader()
{
    const uint2 index = DispatchRaysIndex().xy;
    const float2 size = DispatchRaysDimensions().xy;

    const float2 uv = index / size;
    const float3 target = float3((uv.x * 2.0f - 1.0f) * 1.8f * (size.x / size.y), (1.0f - uv.y) * 4.0f - 2.0f + CAMERA.y, 0.0f);

    RayDesc ray;
    ray.Origin = CAMERA;
    ray.Direction = target - CAMERA;
    ray.TMin = 0.001f;
    ray.TMax = 1000.0f;

    Payload payload;
    payload.missed = false;

    TraceRay(SCENE, RAY_FLAG_NONE, 0xFF, 0, 0, 0, ray, payload);
    RENDER_TARGET[index] = float4(payload.color, 1.0f);
}

[shader("miss")]
void miss_shader(inout Payload payload)
{
    const float slope = normalize(WorldRayDirection()).y;
    const float t = saturate(slope * 5.0f + 0.5f);
    payload.color = lerp(SKY_BOTTOM, SKY_TOP, t);
    payload.missed = true;
}

[shader("closesthit")]
void closest_hit_shader(inout Payload payload, BuiltInTriangleIntersectionAttributes attrib)
{
    switch (InstanceID()) {
    case 0:
        hit_cube(payload, attrib.barycentrics);
        break;
        
    case 1:
        hit_mirror(payload, attrib.barycentrics);
        break;
        
    case 2:
        hit_floor(payload, attrib.barycentrics);
        break;
        
    default:
        payload.color = float3(1.0f, 0.0f, 1.0f);
        break;
    }
}

void hit_cube(inout Payload payload, float2 uv)
{
    const uint triangle_index = PrimitiveIndex() / 2;
    const float3 normal = (triangle_index.xxx % 3 == uint3(0, 1, 2)) * (triangle_index < 3 ? -1 : 1);
    const float3 world_normal = normalize(mul(normal, (float3x3) ObjectToWorld4x3()));

    float3 color = abs(normal) / 3.0f + 0.5f;
    if (uv.x < 0.03f || uv.y < 0.03f) {
        color = 0.25f.xxx;
    }
    color *= saturate(dot(world_normal, normalize(SUN))) + 0.33f;
    payload.color = color;
}

void hit_mirror(inout Payload payload, float2 uv)
{
    const float3 position = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
    const float3 normal = normalize(mul(float3(0.0f, 1.0f, 0.0f), (float3x3) ObjectToWorld4x3()));
    const float3 reflected = reflect(normalize(WorldRayDirection()), normal);

    RayDesc mirror_ray;
    mirror_ray.Origin = position;
    mirror_ray.Direction = reflected;
    mirror_ray.TMin = 0.001f;
    mirror_ray.TMax = 1000.0f;

    TraceRay(SCENE, RAY_FLAG_NONE, 0xFF, 0, 0, 0, mirror_ray, payload);
}

void hit_floor(inout Payload payload, float2 uv)
{
    const float3 position = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();

    const bool2 pattern = frac(position.xz) > 0.5f;
    payload.color = pattern.x ^ pattern.y ? 0.6f.xxx : 0.4f.xxx;

    RayDesc shadow_ray;
    shadow_ray.Origin = position;
    shadow_ray.Direction = SUN - position;
    shadow_ray.TMin = 0.001f;
    shadow_ray.TMax = 1.0f;
    
    Payload shadow_payload;
    shadow_payload.missed = false;
    TraceRay(SCENE, RAY_FLAG_NONE, 0xFF, 0, 0, 0, shadow_ray, shadow_payload);

    if (!shadow_payload.missed) {
        payload.color *= 0.5f;
    }
}
