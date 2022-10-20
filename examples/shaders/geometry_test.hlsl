// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix WMatrix;
    matrix VPMatrix;
    float4 miscData;
};

struct VS_OUT {
    float4 screen : SV_Position;
    float3 world : VS_World;
    float3 normal : VS_Normal;
};

VS_OUT vShader(float3 position : KL_Position, float3 normal : KL_Normal) {
    VS_OUT data;
    data.normal = mul(float4(normal, 0.0f), WMatrix).xyz;
    data.world = mul(float4(position + data.normal * miscData.x, 1.0f), WMatrix).xyz;
    data.screen = mul(float4(data.world, 1.0f), VPMatrix);
    return data;
}

// Geometry shader
[maxvertexcount(3)]
void gShader(triangle VS_OUT inputData[3], inout TriangleStream<VS_OUT> outputStream) {
    for (int i = 0; i < 3; i++) {
        VS_OUT outData = inputData[i];
        outputStream.Append(outData);
    }
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 objectColor;
    float4 sunDirection;
};

float4 pShader(VS_OUT vsData) : SV_Target {
    float3 pixel = objectColor;
    
    float lightIntensity = dot(-sunDirection.xyz, vsData.normal);
    pixel *= lightIntensity;
 
    return float4(pixel, 1.0f);
}
