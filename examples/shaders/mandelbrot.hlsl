float4 STATE_INFO;
float4 FRAME_SIZE;
float4 START_COLOR;

float2 calculate_uv(float2 position)
{
    float2 uv = position / FRAME_SIZE.xy * 2.0f - 1.0f;
    uv.x *= FRAME_SIZE.x / FRAME_SIZE.y;
    uv /= STATE_INFO.z;
    uv += STATE_INFO.xy;
    return uv;
}

float2 square_complex(float2 a)
{
    return float2(a.x * a.x - a.y * a.y, 2.0f * a.x * a.y);
}

float mandel_iterate(float2 position)
{
    float2 num = position;
    for (int i = 1; i <= STATE_INFO.w; i++)
    {
        if (length(num) >= 2.0f)
            return i / STATE_INFO.w;
        num = position + square_complex(num);
    }
    return 0.0f;
}

float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    float2 uv = calculate_uv(position.xy);
    float result = mandel_iterate(uv);
    return START_COLOR * result;
}
