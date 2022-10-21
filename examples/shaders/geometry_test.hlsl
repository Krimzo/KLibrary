// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix w_matrix;
    matrix vp_matrix;
    float4 misc_data;
};

struct vs_out
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float3 normal : VS_Normal;
};

vs_out v_shader(const float3 position : KL_Position, float3 normal : KL_Normal)
{
    vs_out data;
    data.normal = mul(float4(normal, 0.0f), w_matrix).xyz;
    data.world = mul(float4(position + data.normal * misc_data.x, 1.0f), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1.0f), vp_matrix);
    return data;
}

// Geometry shader
[maxvertexcount(3)]
void g_shader(triangle vs_out input_data[3], inout TriangleStream<vs_out> output_stream)
{
    for (int i = 0; i < 3; i++) {
        const vs_out out_data = input_data[i];
        output_stream.Append(out_data);
    }
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 object_color;
    float4 sun_direction;
};

float4 p_shader(const vs_out vs_data) : SV_Target
{
    float3 pixel = object_color;

    const float light_intensity = dot(-sun_direction.xyz, vs_data.normal);
    pixel *= light_intensity;
 
    return float4(pixel, 1.0f);
}
