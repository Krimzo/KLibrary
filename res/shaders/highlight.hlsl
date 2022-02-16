// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix wvp;
}

struct vOut {
    float4 world : SV_POSITION;
};

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    data.world = mul(float4(pos, 1), wvp);

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 color;
}

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel;

    pixel = color;

    return pixel;
}
