
struct vOut {
    float4 ignore : SV_POSITION;
    float4 screen : SCREEN;
    float3 world : WORLD;
    float2 texture : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer VS_CBUFFER : register(b0) {
    matrix world;
    matrix viewProj;
}

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    // Multiplying matrices and interpolating the values
    data.world = mul(float4(pos, 1), world);
    data.screen = mul(float4(data.world, 1), viewProj);
    data.ignore = data.screen;
    data.texture = tex;
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
    pixel = tex0.Sample(samp, data.texture);

    return pixel;
}
