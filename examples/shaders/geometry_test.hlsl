struct VS_OUT
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float3 normal : VS_Normal;
};

float4x4 W;
float4x4 VP;
float4 MISC_DATA;
float4 OBJECT_COLOR;
float4 SUN_DIRECTION;

VS_OUT v_shader(float3 position : KL_Position, float3 normal : KL_Normal)
{
    VS_OUT data;
    data.normal = mul(float4(normal, 0.0f), W).xyz;
    data.world = mul(float4(position + data.normal * MISC_DATA.x, 1.0f), W).xyz;
    data.screen = mul(float4(data.world, 1.0f), VP);
    return data;
}

[maxvertexcount(3)]
void g_shader(triangle VS_OUT input_data[3], inout TriangleStream<VS_OUT> output_stream)
{
    for (int i = 0; i < 3; i++)
    {
        VS_OUT out_data = input_data[i];
        output_stream.Append(out_data);
    }
}

float4 p_shader(VS_OUT vs_data) : SV_Target0
{
    float light_intensity = saturate(dot(-SUN_DIRECTION.xyz, vs_data.normal));
    return OBJECT_COLOR * light_intensity;
}
