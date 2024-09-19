struct ExampleStruct
{
    float3 position;
};

RWStructuredBuffer<ExampleStruct> DATA_BUFFER : register(u0);

[numthreads(1, 1, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    DATA_BUFFER[thread_id.x].position = thread_id.x;
}
