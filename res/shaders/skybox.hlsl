// Vertex shader
struct vOut {
    float4 world : SV_POSITION;
    float3 textur : TEX;
};

cbuffer SB_CB : register(b0) {
    matrix vp;
}

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    // Vertex transform
    data.world = mul(float4(pos, 0), vp).xyww;
    data.textur = pos;

    return data;
}

// Pixel shader
SamplerState samp : register(s0);
TextureCube tex0 : register(t0);

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel;

    pixel = tex0.Sample(samp, data.textur);

    return pixel;
}
