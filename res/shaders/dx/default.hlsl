
struct vOut {
    float4 pos : SV_POSITION;
    float3 wPos : WORLD;
    float2 tex : TEXCOORD;
    float3 norm : NORMAL;
};

cbuffer VS_CBUFFER : register(b0) {
    matrix world;
    matrix viewProj;
}

vOut vShader(float3 pos : POSIN, float2 tex : TEXIN, float3 norm : NORMIN) {
    vOut dataOut;

    dataOut.wPos = mul(float4(pos, 1), world);
    dataOut.pos = mul(float4(dataOut.wPos, 1), viewProj);
    dataOut.tex = tex;
    dataOut.norm = mul(float4(norm, 0), world);

	return dataOut;
}

SamplerState samp : register(s0);
Texture2D tex0 : register(t0);

float4 pShader(vOut dataIn) : SV_TARGET {
    float4 pixel;

    dataIn.norm = normalize(dataIn.norm);

    pixel = tex0.Sample(samp, dataIn.tex);

	return pixel;
}
