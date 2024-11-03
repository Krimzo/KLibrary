#include "examples.h"


int examples::hello_world_ext_12_main(const int argc, const char** argv)
{
    kl::Window window{ "Hardware Raytracing" };
    kl::GPU12 gpu{ window.ptr() };
    kl::Timer timer;

    kl::dx12::DescriptorHeap uav_heap{};
    kl::dx12::Resource ray_target{};
    
    window.on_resize.emplace_back([&](kl::Int2 size)
    {
        ray_target = {};
        uav_heap = {};
        gpu.resize(size);
        
        const D3D12_RESOURCE_DESC target_descriptor{
            .Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            .Width = (UINT) size.x,
            .Height = (UINT) size.y,
            .DepthOrArraySize = 1,
            .MipLevels = 1,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .SampleDesc{
                .Count = 1,
                .Quality = 0,
            },
            .Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
        };
        ray_target = gpu.create_commited_resource(&target_descriptor, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    
        constexpr D3D12_UNORDERED_ACCESS_VIEW_DESC uav_descriptor{
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D,
        };
        uav_heap = gpu.create_descriptor_heap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
        gpu.device()->CreateUnorderedAccessView(ray_target.get(), nullptr, &uav_descriptor, uav_heap->GetCPUDescriptorHandleForHeapStart());
    });
    window.maximize();
    
    const std::vector quad_mesh_data = kl::parse_obj_file("meshes/quad.obj");
    const std::vector cube_mesh_data = kl::parse_obj_file("meshes/cube.obj");
    const std::vector monke_mesh_data = kl::parse_obj_file("meshes/monke.obj");
    
    const kl::dx12::Resource quad_vb = gpu.create_buffer(quad_mesh_data.data(), (UINT) quad_mesh_data.size() * sizeof(kl::Vertex), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
    const kl::dx12::Resource cube_vb = gpu.create_buffer(cube_mesh_data.data(), (UINT) cube_mesh_data.size() * sizeof(kl::Vertex), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
    const kl::dx12::Resource monke_vb = gpu.create_buffer(monke_mesh_data.data(), (UINT) monke_mesh_data.size() * sizeof(kl::Vertex), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

    const kl::dx12::AccelerationStructure quad_blas = gpu.create_triangle_blas(quad_vb, sizeof(kl::Vertex));
    const kl::dx12::AccelerationStructure cube_blas = gpu.create_triangle_blas(cube_vb, sizeof(kl::Vertex));
    const kl::dx12::AccelerationStructure monke_blas = gpu.create_triangle_blas(monke_vb, sizeof(kl::Vertex));
    
    D3D12_RAYTRACING_INSTANCE_DESC instance_data[3] = {};
    instance_data[0] = {
        .InstanceID = 0,
        .InstanceMask = 1,
        .AccelerationStructure = monke_blas->GetGPUVirtualAddress(),
    };
    instance_data[1] = {
        .InstanceID = 1,
        .InstanceMask = 1,
        .AccelerationStructure = cube_blas->GetGPUVirtualAddress(),
    };
    instance_data[2] = {
        .InstanceID = 2,
        .InstanceMask = 1,
        .AccelerationStructure = quad_blas->GetGPUVirtualAddress(),
    };

    kl::Float4x4 default_mat{};
    memcpy(&instance_data[0].Transform, &default_mat, sizeof(float) * 12);
    memcpy(&instance_data[1].Transform, &default_mat, sizeof(float) * 12);
    memcpy(&instance_data[2].Transform, &default_mat, sizeof(float) * 12);
    const kl::dx12::Resource instances = gpu.create_upload_buffer(instance_data, sizeof(instance_data));

    UINT64 update_scratch_size = 0;
    const kl::dx12::AccelerationStructure tlas = gpu.create_tlas(instances, &update_scratch_size);
    const kl::dx12::Resource tlas_update_scratch = gpu.create_commited_resource(update_scratch_size,
        D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
    
    constexpr D3D12_DESCRIPTOR_RANGE uav_range{
        .RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
        .NumDescriptors = 1,
    };
    const D3D12_ROOT_PARAMETER root_parameter0{
        .ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        .DescriptorTable{
            .NumDescriptorRanges = 1,
            .pDescriptorRanges = &uav_range,
        },
    };
    constexpr D3D12_ROOT_PARAMETER root_parameter1{
       .ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV,
       .Descriptor{
           .ShaderRegister = 0,
           .RegisterSpace = 0,
       },
    };
    const kl::dx12::RootSignature root_signature = gpu.create_root_signature({ root_parameter0, root_parameter1 }, {}, D3D12_ROOT_SIGNATURE_FLAG_NONE);
    
    const std::string compiled_shader = kl::read_file("shaders/raytracing/raytracing.cso");
    const kl::dx12::StateObject pipeline_state = gpu.create_default_raytracing_pipeline(compiled_shader, root_signature);

    kl::dx12::ObjectProperties pipeline_properties{};
    pipeline_state.as(pipeline_properties);

    byte shader_id_buffer[D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT * 3] = {};
    memcpy(shader_id_buffer + D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT * 0, pipeline_properties->GetShaderIdentifier(L"ray_generation_shader"), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	memcpy(shader_id_buffer + D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT * 1, pipeline_properties->GetShaderIdentifier(L"miss_shader"), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	memcpy(shader_id_buffer + D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT * 2, pipeline_properties->GetShaderIdentifier(L"hit_group"), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
    const kl::dx12::Resource shader_ids = gpu.create_upload_buffer(shader_id_buffer, sizeof(shader_id_buffer));

    while (window.process()) {
        timer.update();
        
        if (window.keyboard.esc) {
            window.close();
        }

        const float elapsed_t = timer.elapsed();
        kl::Float4x4 cube = kl::Float4x4::translation({ -1.5f, 2.0f, 2.0f });
        cube *= kl::Float4x4::rotation(kl::Float3(elapsed_t / 2.0f, elapsed_t / 3.0f, elapsed_t / 5.0f) * kl::to_degrees());
        memcpy(&instance_data[0].Transform, &cube, sizeof(float) * 12);
        kl::Float4x4 mirror = kl::Float4x4::translation({ 2.0f, 2.0f, 2.0f });
        mirror *= kl::Float4x4::rotation(kl::Float3(-1.8f, std::sin(elapsed_t) / 8.0f + 1.0f, 0.0f) * kl::to_degrees());
        memcpy(&instance_data[1].Transform, &mirror, sizeof(float) * 12);
        kl::Float4x4 floor = kl::Float4x4::translation({ 0.0f, 0.0f, 2.0f });
        floor *= kl::Float4x4::scaling({ 5.0f, 5.0f, 5.0f });
        memcpy(&instance_data[2].Transform, &floor, sizeof(float) * 12);
        gpu.copy(instances, instance_data, sizeof(instance_data));

        gpu.execute([&](auto& commands)
        {
            commands.update_tlas(tlas, tlas_update_scratch, instances);

            commands.list->SetPipelineState1(pipeline_state.get());
            commands.list->SetComputeRootSignature(root_signature.get());
            commands.list->SetComputeRootDescriptorTable(0, uav_heap->GetGPUDescriptorHandleForHeapStart());
            commands.list->SetComputeRootShaderResourceView(1, tlas->GetGPUVirtualAddress());
            ID3D12DescriptorHeap* uav_heaps[1] = { uav_heap.get() };
            commands.list->SetDescriptorHeaps(1, uav_heaps);

            const D3D12_RESOURCE_DESC target_descriptor = ray_target->GetDesc();
            commands.dispatch_rays(shader_ids->GetGPUVirtualAddress(), (UINT) target_descriptor.Width, (UINT) target_descriptor.Height);

            auto back_buffer = gpu.get_back_buffer(gpu.back_buffer_index());
            commands.transition_resource(ray_target, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
            commands.transition_resource(back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST);
            commands.copy(back_buffer, ray_target);
            commands.transition_resource(back_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
            commands.transition_resource(ray_target, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
        });
        
        gpu.swap_buffers(true);
    }
    return 0;
}
