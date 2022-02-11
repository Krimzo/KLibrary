// Vertex shader
struct vOut {    
    float4 world : SV_POSITION;
    float2 textur : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer VS_BUFF : register(b0) {
    float aspect;
    float xPos;
    float yPos;
    float ignore;
};

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    data.world = float4((pos.x + xPos) / aspect, pos.y, pos.z, 1.0f);
    data.textur = tex;
    data.normal = norm;

    return data;
}

// Pixel shader
SamplerState samp : register(s0);
Texture2D tex0 : register(t0);

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel;

    pixel = tex0.Sample(samp, data.textur);

    return pixel;
}
