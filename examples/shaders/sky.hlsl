// Vertex shader
float4 v_shader(float3 pos : KL_Position) : SV_Position
{
    return float4(pos, 1);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 frame_size;
    matrix inverse_camera;
    float4 camera_position;
    float4 sun_direction;
}

static const float3 sky_top_color = { 0.62f, 0.77f, 0.88f };
static const float3 sky_bottom_color = { 0.89f, 0.93f, 0.96f };
static const float3 sun_sky_color = { 0.98f, 0.9f, 0.76f };
static const float2 sun_size = { 0.75f, 1.55f };

float4 p_shader(float4 screen : SV_Position) : SV_Target
{
    float3 pixel = 0;
    
    float2 ndc = float2(screen.x, frame_size.y - screen.y) / float2(frame_size.x, frame_size.y);
    ndc = ndc * 2 - 1;
	
    float4 ray = mul(float4(ndc, 1, 1), inverse_camera);
    ray /= ray.w;

    const float3 ray_direction = normalize(ray.xyz);

    const float sky_mix_value = (dot(-ray_direction, float3(0, 1, 0)) + 1) * 0.5f;
    pixel = lerp(sky_top_color, sky_bottom_color, sky_mix_value);
    pixel *= dot(-sun_direction.xyz, float3(0, 1, 0));
		
    const float sun_angle = degrees(acos(dot(ray_direction, -sun_direction.xyz)));
    pixel = lerp(sun_sky_color, pixel, smoothstep(sun_size.x, sun_size.y, sun_angle));
    
    return float4(pixel, 1);
}
