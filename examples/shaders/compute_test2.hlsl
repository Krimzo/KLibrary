// Compute shader
struct ExampleStruct {
    float3 position;
};

RWStructuredBuffer<ExampleStruct> DataMap : register(u0);

[numthreads(1, 1, 1)]
void cShader(uint3 ID : SV_DispatchThreadID) {
    DataMap[ID.x].position = ID.x;
}
