
struct vIn {
    float3 pos : POSIN;
	float2 tex : TEXIN;
    float3 norm : NORMIN;
};

struct vOut {
    float4 pos : SV_POSITION;
    float2 sPos : SCREEN;
    float2 tex : TEXCOORD;
    float3 norm : NORMAL;
};

vOut vShader(vIn dataIn) {
    vOut dataOut;

    dataOut.pos = float4(dataIn.pos, 1);
    dataOut.sPos = dataIn.pos.xy;
    dataOut.tex = dataIn.tex;
    dataOut.norm = dataIn.norm;

	return dataOut;
}

float4 pShader(vOut dataIn) : SV_TARGET {
    float4 pixel;

    pixel = float4(dataIn.norm, 1);

	return pixel;
}
