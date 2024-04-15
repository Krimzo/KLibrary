// Vertex shader
float4 v_shader(float3 pos : KL_Position) : SV_Position
{
    return float4(pos, 1);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 state_info;
    float4 frame_size;
    float4 start_color;
}

float2 calculate_uv(const float2 position)
{
    float2 uv = position / frame_size.xy * 2 - 1;
    uv.x *= frame_size.x / frame_size.y;
    uv /= state_info.z;
    uv += state_info.xy;
    return uv;
}

float2 square_complex(const float2 a)
{
    return float2(a.x * a.x - a.y * a.y, 2.0f * a.x * a.y);
}

float mandel_iterate(const float2 position)
{
    float2 num = position;
    for (int i = 1; i <= state_info.w; i++) {
        if (length(num) > 2.0f) {
            return i / state_info.w;
        }
        num = position + square_complex(num);
    }
    return 0;
}

float4 p_shader(float4 screen : SV_Position) : SV_Target
{
    const float4 pixel = start_color;
    const float2 uv = calculate_uv(screen.xy);
    const float result = mandel_iterate(uv);
    return pixel * result;
}
