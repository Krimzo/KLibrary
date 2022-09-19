// Vertex shader
float4 vShader(float3 pos : KL_Position) : SV_Position {
    return float4(pos, 1);
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 stateInfo;
    float4 frameSize;
    float4 startColor;
}

float2 CalculateUV(float2 pos) {
    float2 uv = (pos / frameSize.xy * 2 - 1);
    uv.x *= frameSize.x / frameSize.y;
    uv /= stateInfo.z;
    uv += stateInfo.xy;
    return uv;
}

float2 ComplexSqr(float2 a) {
    return float2(a.x * a.x - a.y * a.y, 2 * a.x * a.y);
}

float MandelIterate(float2 pos) {
    float2 num = pos;
    for (int i = 1; i <= stateInfo.w; i++) {
        if (length(num) > 2) {
            return i / stateInfo.w;
        }
        num = pos + ComplexSqr(num);
    }
    return 0;
}

float4 pShader(float4 screen : SV_Position) : SV_Target {
    float4 pixel = startColor;
    float2 uv = CalculateUV(screen.xy);
    float result = MandelIterate(uv);
    return pixel * result;
}
