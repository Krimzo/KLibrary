// Compute shader
cbuffer CS_CB
{
    float4 misc_data;
};

RWTexture2D<float4> write_map : register(u0);

[numthreads(32, 32, 1)]
void c_shader(const uint3 thread_id : SV_DispatchThreadID)
{
    if (thread_id.x <= (uint) misc_data.x && thread_id.y <= (uint) misc_data.y) {
        write_map[thread_id.xy] = float4(0.27f, 0.56f, 0.44f, 1.0f);
    }
}

// Vertex shader
float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

// Pixel shader
Texture2D read_map : register(t0);

float4 p_shader(float4 position : SV_Position) : SV_Target
{
    return read_map[position.xy];
}
