// Vertex shader
static const float PI = 3.14159265359f;

cbuffer VS_CB : register(b0) {
    matrix WMatrix;
    matrix VPMatrix;
    float4 timeData;
};

struct VS_OUT {
    float4 screen : SV_Position;
    float3 world : VS_World;
    float3 normal : VS_Normal;
    float3 color : VS_Color;
};

float3 PlaneToSphere(float3 position, float halfPlaneSize) {
    float2 ndc = position.xz / halfPlaneSize;
    float2 coords = radians(ndc * float2(180, 90));
    
    float R = 1 + position.y;
    return float3(
        R * cos(coords.y) * cos(coords.x),
        R * cos(coords.y) * sin(coords.x),
        R * sin(coords.y + PI)
    );
}

VS_OUT vShader(float3 position : KL_Position) {
    VS_OUT data;
    
    position.y = sin(position.x * 25 + timeData.x) * 0.025f;
    
    position = PlaneToSphere(position, 5);
    
    data.world = mul(float4(position, 1), WMatrix).xyz;
    data.screen = mul(float4(data.world, 1), VPMatrix);
    data.color = 1;
    
    return data;
}

// Geometry shader
[maxvertexcount(3)]
void gShader(triangle VS_OUT inputData[3], inout TriangleStream<VS_OUT> outputStream) {
    float3 AB = inputData[1].world - inputData[0].world;
    float3 AC = inputData[2].world - inputData[0].world;
    float3 updatedNormal = normalize(cross(AB, AC));
    
    for (int i = 0; i < 3; i++) {
        VS_OUT outData = inputData[i];
        outData.normal = updatedNormal;
        outputStream.Append(outData);
    }
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 sunDirection;
};

float4 pShader(VS_OUT vsData) : SV_Target{
    float3 pixel = vsData.color;
    
    vsData.normal = normalize(vsData.normal);

    float lightIntensity = dot(-sunDirection.xyz, vsData.normal);
    pixel *= lightIntensity;

    return float4(pixel, 1);
}
