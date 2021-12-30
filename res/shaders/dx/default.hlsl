
struct vIn {
    float3 pos : POSIN;
	float2 tex : TEXIN;
    float3 norm : NORMIN;
};

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

vOut vShader(vIn dataIn) {
    vOut dataOut;

    dataOut.wPos = mul(float4(dataIn.pos, 1), world);
    dataOut.pos = mul(float4(dataOut.wPos, 1), viewProj);
    dataOut.tex = dataIn.tex;
    dataOut.norm = mul(float4(dataIn.norm, 0), world);

	return dataOut;
}

float4 pShader(vOut dataIn) : SV_TARGET {
    float4 pixel;

    dataIn.norm = normalize(dataIn.norm);

    //if (dataIn.wPos.y > 0) {
    //    pixel = float4(1, 1, 1, 1);
    //}
    //else {
    //    pixel = float4(0.0784313, 0.862745, 0.627450, 1);
    //}

    pixel = float4(dataIn.norm, 1);

	return pixel;
}
