// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix wvp;
}

struct VS_OUT {
    float4 world : SV_POSITION;
};

VS_OUT vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    VS_OUT data;

    data.world = mul(float4(pos, 1), wvp);

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 objColor;
    float4 objIndex;
}

struct PS_OUT {
    float4 color : SV_TARGET0;
    float4 index : SV_TARGET1;
};

PS_OUT pShader(VS_OUT data) {
    PS_OUT output;

    output.color = objColor;
    output.index = objIndex;

    return output;
}
