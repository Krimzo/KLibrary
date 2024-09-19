float4 MISC_DATA;

RWTexture2D<float4> TARGET_TEXTURE : register(u0);

Texture2D SOURCE_TEXTURE : register(t0);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    if (thread_id.x <= (uint) MISC_DATA.x && thread_id.y <= (uint) MISC_DATA.y)
        TARGET_TEXTURE[thread_id.xy] = float4(0.25f, 0.55f, 0.45f, 1.0f);
}

float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    return SOURCE_TEXTURE[position.xy];
}
