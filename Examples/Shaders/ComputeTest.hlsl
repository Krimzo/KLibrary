// Compute shader
cbuffer CS_CB {
    float4 miscData;
};

RWTexture2D<float4> WriteMap : register(u0);

[numthreads(32, 32, 1)]
void cShader(uint3 ID : SV_DispatchThreadID) {
    if (ID.x <= miscData.x && ID.y <= miscData.y) {
        WriteMap[ID.xy] = float4(0.27f, 0.56f, 0.44f, 1.0f);
    }
}

// Vertex shader
float4 vShader(float3 position : KL_Position) : SV_Position {
    return float4(position, 1.0f);
}

// Pixel shader
Texture2D ReadMap : register(t0);

float4 pShader(float4 position : SV_Position) : SV_Target {
    return ReadMap[position.xy];
}
