// Vertex shader
struct vOut {
    float4 world : SV_POSITION;
    float2 textur : TEX;
    float3 normal : NORM;
};

cbuffer VS_CB : register(b0) {
    matrix w;
    matrix vp;
}

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    // WVP calculation
    matrix wvp = mul(w, vp);

    // Vertex transform
    data.world = mul(float4(pos, 1), wvp);
    data.textur = tex;
    data.normal = mul(float4(norm, 0), w).xyz;

    return data;
}

// Pixel shader
SamplerState samp : register(s0);
Texture2D tex0 : register(t0);

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel;

    // Normalizing the normal
    data.normal = normalize(data.normal);

    // Getting the texture pixel
    pixel = tex0.Sample(samp, data.textur);

    return pixel;
}
