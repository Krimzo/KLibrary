// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix WMatrix;
    matrix VPMatrix;
    float4 timeData;
    float2 coords;
    float2 zoom;
};

struct VS_OUT {
    float4 screen : SV_Position;
    float3 world : VS_World;
    float3 normal : VS_Normal;
    float3 color : VS_Color;
};

float2 ComplexSqr(float2 a) {
    return float2(a.x * a.x - a.y * a.y, 2.0f * a.x * a.y);
}

VS_OUT vShader(float3 position : KL_Position) {
    VS_OUT data;
    
    position.y = 1.0f;
    data.color = 1.0f;
    
    float2 uv = position.xz / 5.0f;
    uv /= zoom.x;
    uv += coords;
    
    float n = 64.0f;
    float2 num = uv;
    for (int i = 0; i < n; i++) {
        if (length(num) > 2.0f) {
            float iOn = i / n;
            position.y *= iOn;
            data.color *= iOn;
            break;
        }
		
        num = ComplexSqr(num) + uv;
    }
    
    //position.y = sin(position.x + timeData.x) * sin(position.z);
    
    data.world = mul(float4(position, 1), WMatrix).xyz;
    data.screen = mul(float4(data.world, 1), VPMatrix);
    //data.color = 1.0f;
    
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
