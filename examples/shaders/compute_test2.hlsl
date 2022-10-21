// Compute shader
struct example_struct
{
    float3 position;
};

RWStructuredBuffer<example_struct> data_map : register(u0);

[numthreads(1, 1, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    data_map[thread_id.x].position = thread_id.x;
}
