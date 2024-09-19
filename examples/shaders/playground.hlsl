struct VS_OUT
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float3 normal : VS_Normal;
    float3 color : VS_Color;
};

static const float PI = 3.14159265359f;

float4x4 W;
float4x4 VP;
float4 TIME_DATA;
float4 SUN_DIRECTION;

float normalized_time_sin(float value)
{
    return value * (sin(TIME_DATA.x) + 1) * 0.5f;
}

float3 plane_to_sphere(float3 position, float half_plane_size)
{
    float2 ndc = position.xz / half_plane_size;
    float2 coords = radians(ndc * float2(180, 90));

    float r = 1 + position.y;
    return float3(
        r * cos(coords.y) * cos(coords.x),
        r * cos(coords.y) * sin(coords.x),
        r * sin(coords.y + PI)
    );
}

void alter_position(inout float3 position)
{
    position.y = sin(1.5f * position.x + TIME_DATA.x);
}

VS_OUT v_shader(float3 position : KL_Position)
{
    VS_OUT data;
    alter_position(position);
    data.world = mul(float4(position, 1.0f), W).xyz;
    data.screen = mul(float4(data.world, 1.0f), VP);
    data.normal = 0.0f;
    data.color = 1.0f;
    return data;
}

[maxvertexcount(3)]
void g_shader(triangle VS_OUT input_data[3], inout TriangleStream<VS_OUT> output_stream)
{
    float3 ab = input_data[1].world - input_data[0].world;
    float3 ac = input_data[2].world - input_data[0].world;
    float3 updated_normal = normalize(cross(ab, ac));
    for (int i = 0; i < 3; i++)
    {
        VS_OUT out_data = input_data[i];
        out_data.normal = updated_normal;
        output_stream.Append(out_data);
    }
}

float4 p_shader(VS_OUT vs_data) : SV_Target0
{
    vs_data.normal = normalize(vs_data.normal);
    float3 pixel = vs_data.color;
    pixel *= saturate(dot(-SUN_DIRECTION.xyz, vs_data.normal));
    return float4(pixel, 1.0f);
}
