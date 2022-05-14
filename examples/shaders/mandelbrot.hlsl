// Vertex shader
float4 vShader(float3 pos : POS_IN) : SV_POSITION
{
    return float4(pos, 1.0f);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float2 frameSize;
    float2 zoom;
    float2 pos;
    float2 startPos;
}

// Computes a complex number power
float2 ComplexSqr(float2 a)
{
    return float2(a.x * a.x - a.y * a.y, 2.0f * a.x * a.y);
}

float4 pShader(float4 screen : SV_POSITION) : SV_TARGET
{
    float4 pixel = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Getting the uv coordinates
    float2 uv = (screen.xy / frameSize * 2.0f - 1.0f) * float2(frameSize.x / frameSize.y, 1.0f);

    // Setting the zoom
    uv /= zoom.x;

    // Setting the position
    uv += pos;
    
    // Calculating the reaction
    float n = 64.0f;
    float2 num = startPos.x + uv;
    for (int i = 0; i < n; i++)
    {
        // Checking the distance
        if (length(num) > 2.0f)
        {
            pixel *= (i / n);
            break;
        }

        // Computing the next point
        num = ComplexSqr(num) + uv;
    }
    
    return pixel;
}
