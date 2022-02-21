// Vertex shader
struct vOut {
    float4 world : SV_POSITION;
    float4 color : COL;
};

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    data.world = float4(pos, 1);
    data.color = float4(norm, 1);

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 mouse;
};

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel = float4(0, 0, 0, 0);

    if (data.world.x < mouse.x) {
        pixel = data.color;
    }

    return pixel;
}
