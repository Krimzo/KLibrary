// Compue shader
struct ExampleStruct {
    float3 position;
};

RWStructuredBuffer<ExampleStruct> DataMap : register(u0);

[numthreads(5, 1, 1)]
void cShader(uint3 ID : SV_DispatchThreadID) {
    DataMap[ID.x].position = ID.x;
}
