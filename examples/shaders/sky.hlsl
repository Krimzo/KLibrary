static const float3 SKY_TOP = { 0.62f, 0.77f, 0.88f };
static const float3 SKY_BOTTOM = { 0.89f, 0.93f, 0.96f };
static const float3 SUN_COLOR = { 0.98f, 0.9f, 0.76f };
static const float2 SUN_SIZE = { 0.75f, 1.55f };

float4x4 INVERSE_CAMERA;
float4 FRAME_SIZE;
float4 SUN_DIRECTION;

float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    float2 ndc = float2(position.x, FRAME_SIZE.y - position.y) / float2(FRAME_SIZE.x, FRAME_SIZE.y);
    ndc = ndc * 2.0f - 1.0f;
	
    float4 ray = mul(float4(ndc, 1.0f, 1.0f), INVERSE_CAMERA);
    ray /= ray.w;
    
    float3 ray_direction = normalize(ray.xyz);
    float sun_angle = degrees(acos(dot(ray_direction, -SUN_DIRECTION.xyz)));
    float sky_mix_value = (dot(-ray_direction, float3(0.0f, 1.0f, 0.0f)) + 1.0f) * 0.5f;

    float3 pixel = 0.0f;
    pixel = lerp(SKY_TOP, SKY_BOTTOM, sky_mix_value);
    pixel *= saturate(dot(-SUN_DIRECTION.xyz, float3(0.0f, 1.0f, 0.0f)));
    pixel = lerp(SUN_COLOR, pixel, smoothstep(SUN_SIZE.x, SUN_SIZE.y, sun_angle));
    return float4(pixel, 1.0f);
}
