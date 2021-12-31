
struct vOut {
    float4 ignore_pos : SV_POSITION;
    float4 screen_pos : SCREEN;
    float3 world_pos : WORLD;
    float2 texture_pos : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer VS_CBUFFER : register(b0) {
    matrix world;
    matrix viewProj;
}

vOut vShader(float3 pos : POSIN, float2 tex : TEXIN, float3 norm : NORMIN) {
    vOut data;

    // Multiplying matrices and interpolating the values
    data.world_pos = mul(float4(pos, 1), world);
    data.screen_pos = mul(float4(data.world_pos, 1), viewProj);
    data.ignore_pos = data.screen_pos;
    data.texture_pos = tex;
    data.normal = mul(float4(norm, 0), world);

	return data;
}

SamplerState samp : register(s0);
Texture2D tex0 : register(t0);

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel;

    // Normalizing the normal
    data.normal = normalize(data.normal);

    // Computing the pixel color
    pixel = tex0.Sample(samp, data.texture_pos);

    return pixel;
}
