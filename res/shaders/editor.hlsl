// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix w;
    matrix vp;
}

struct VS_OUT {
    float4 world : SV_POSITION;
    float2 textur : TEX;
    float3 normal : NORM;
};

VS_OUT vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    VS_OUT data;

    // WVP calculation
    matrix wvp = mul(w, vp);

    // Vertex transform
    data.world = mul(float4(pos, 1), wvp);
    data.textur = tex;
    data.normal = mul(float4(norm, 0), w).xyz;

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 ambient;
    float4 dirCol;
    float4 dirDir;
    float4 objIndex;
}

SamplerState samp : register(s0);
Texture2D tex0 : register(t0);

struct PS_OUT {
    float4 color : SV_TARGET0;
    float index : SV_TARGET1;
};

PS_OUT pShader(VS_OUT data) {
    PS_OUT output;

    // Texture color
    float3 textureColor = tex0.Sample(samp, data.textur).xyz;

    // Ambient light color
    float3 ambientColor = ambient.xyz;

    // Directional light color
    float3 directColor = float3(0, 0, 0);

    // Calculating the directional light intensity
    float diffuseFactor = dot(-dirDir.xyz, normalize(data.normal));

    // Checking the diffuse factor
    if (diffuseFactor > 0) {
        directColor = (dirCol * diffuseFactor).xyz;
    }

    // Setting the pixel color
    output.color = float4(textureColor * (directColor + ambientColor), 1);

    // Setting the index
    output.index = objIndex.x;

    // Returning
    return output;
}
