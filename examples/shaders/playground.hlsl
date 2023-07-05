// Vertex shader
static const float PI = 3.14159265359f;

cbuffer VS_CB : register(b0)
{
    matrix w_matrix;
    matrix vp_matrix;
    float4 time_data;
};

struct vs_out
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float3 normal : VS_Normal;
    float3 color : VS_Color;
};

float normalized_time_sin(const float value)
{
    return value * (sin(time_data.x) + 1) * 0.5f;
}

float3 plane_to_sphere(float3 position, const float half_plane_size)
{
    const float2 ndc = position.xz / half_plane_size;
    float2 coords = radians(ndc * float2(180, 90));

    const float r = 1 + position.y;
    return float3(
        r * cos(coords.y) * cos(coords.x),
        r * cos(coords.y) * sin(coords.x),
        r * sin(coords.y + PI)
    );
}

float3 alter_position(float3 position)
{
    position.y = sin(1.5f * position.x + time_data.x);
    //position.y *= normalized_time_sin(1);
    //position = plane_to_sphere(position, 5);
    return position;
}

vs_out v_shader(float3 position : KL_Position)
{
    vs_out data;
    
    position = alter_position(position);
    
    data.world = mul(float4(position, 1), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1), vp_matrix);
    data.normal = 0;
    data.color = 1;
    
    return data;
}

// Geometry shader
[maxvertexcount(3)]
void g_shader(triangle vs_out input_data[3], inout TriangleStream<vs_out> output_stream)
{
    const float3 ab = input_data[1].world - input_data[0].world;
    const float3 ac = input_data[2].world - input_data[0].world;
    const float3 updated_normal = normalize(cross(ab, ac));
    
    for (int i = 0; i < 3; i++) {
        vs_out out_data = input_data[i];
        out_data.normal = updated_normal;
        output_stream.Append(out_data);
    }
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 sun_direction;
};

float4 p_shader(vs_out vs_data) : SV_Target
{
    float3 pixel = vs_data.color;
    
    vs_data.normal = normalize(vs_data.normal);

    const float light_intensity = dot(-sun_direction.xyz, vs_data.normal);
    pixel *= light_intensity;

    return float4(pixel, 1);
}
