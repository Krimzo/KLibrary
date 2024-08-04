#include "examples.h"


/* HELPER SOURCE: https://landelare.github.io/2023/02/18/dxr-tutorial.html (thanks to the author) */

int examples::hello_world_ext_12_main(const int argc, const char** argv)
{
    // Window setup
    kl::Window window{ "Tracing", { 1600, 900 } };
    kl::GPU12 gpu{ (HWND) window, kl::IS_DEBUG };
    kl::Timer timer{};

    // Window resize setup
    kl::dx12::DescriptorHeap uav_heap{};
    kl::dx12::Resource render_target{};

    window.on_resize.emplace_back([&](const kl::Int2 size)
    {
        if (size.x < 1 || size.y < 1) {
            return;
        }

        render_target = {};
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
        render_target = gpu.create_commited_resource(&target_descriptor, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

        constexpr D3D12_UNORDERED_ACCESS_VIEW_DESC uav_descriptor{
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D,
        };
        uav_heap = gpu.create_descriptor_heap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
        gpu.device()->CreateUnorderedAccessView(render_target.get(), nullptr, &uav_descriptor, uav_heap->GetCPUDescriptorHandleForHeapStart());
    });
    window.maximize();

    // Mesh setup
    const std::vector quad_mesh_data = kl::parse_obj_file("meshes/quad.obj");
    const std::vector cube_mesh_data = kl::parse_obj_file("meshes/cube.obj");
    const std::vector monke_mesh_data = kl::parse_obj_file("meshes/monke.obj");

    const kl::dx12::Resource quad_vb = gpu.create_upload_buffer(quad_mesh_data.data(), (UINT) quad_mesh_data.size() * sizeof(kl::Vertex<float>));
    const kl::dx12::Resource cube_vb = gpu.create_upload_buffer(cube_mesh_data.data(), (UINT) cube_mesh_data.size() * sizeof(kl::Vertex<float>));
    const kl::dx12::Resource monke_vb = gpu.create_upload_buffer(monke_mesh_data.data(), (UINT) monke_mesh_data.size() * sizeof(kl::Vertex<float>));

    const kl::dx12::Resource quad_blas = gpu.create_triangle_blas(quad_vb);
    const kl::dx12::Resource cube_blas = gpu.create_triangle_blas(cube_vb);
    const kl::dx12::Resource monke_blas = gpu.create_triangle_blas(monke_vb);

    // Scene setup
    constexpr UINT INSTANCE_COUNT = 3;
    const kl::dx12::Resource instances = gpu.create_commited_resource(sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * INSTANCE_COUNT, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_HEAP_TYPE_UPLOAD);

    {
        D3D12_RAYTRACING_INSTANCE_DESC* instance_data = nullptr;
        instances->Map(0, nullptr, reinterpret_cast<void**>(&instance_data)) >> kl::verify_result;
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
        instances->Unmap(0, nullptr);
    }

    // TLAS
    UINT64 update_scratch_size = 0;
    const kl::dx12::Resource tlas = gpu.create_tlas(instances, INSTANCE_COUNT, &update_scratch_size);
    const kl::dx12::Resource tlas_update_scratch = gpu.create_commited_resource(update_scratch_size, D3D12_RESOURCE_STATE_COMMON);

    // Root signature setup
    const D3D12_DESCRIPTOR_RANGE uav_range{
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
    const D3D12_ROOT_PARAMETER root_parameter1{
       .ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV,
       .Descriptor{
           .ShaderRegister = 0,
           .RegisterSpace = 0,
       },
    };
    const kl::dx12::RootSignature root_signature = gpu.create_root_signature({ root_parameter0, root_parameter1 }, {}, D3D12_ROOT_SIGNATURE_FLAG_NONE);

    // Pipeline setup
    const std::vector compiled_shader = kl::read_file("shaders/raytracing/raytracing.cso");
    const kl::dx12::StateObject pipeline_state = gpu.create_default_raytracing_pipeline(compiled_shader, root_signature);

    constexpr UINT64 SHADER_COUNT = 3;
    const kl::dx12::Resource shader_ids = gpu.create_commited_resource(D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT * SHADER_COUNT, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_HEAP_TYPE_UPLOAD);

    {
        kl::dx12::ObjectProperties max_payload_size_properties{};
        pipeline_state->QueryInterface<ID3D12StateObjectProperties>(&max_payload_size_properties) >> kl::verify_result;

        void* shader_data = nullptr;
        shader_ids->Map(0, nullptr, &shader_data) >> kl::verify_result;
        const auto write_id = [&](const wchar_t* name)
        {
            const void* id = max_payload_size_properties->GetShaderIdentifier(name);
            memcpy(shader_data, id, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
            shader_data = static_cast<char*>(shader_data) + D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT;
        };
        write_id(L"ray_generation_shader");
        write_id(L"miss_shader");
        write_id(L"hit_group");
        shader_ids->Unmap(0, nullptr);
    }

    // Render loop
    while (window.process(false)) {
        // Prepare
        timer.update_delta();
        const float elapsed_t = timer.elapsed();
        const float delta_t = timer.delta();
        gpu.reset();

        // Input
        if (window.keyboard.esc) {
            window.close();
        }

        // Update entities
        {
            D3D12_RAYTRACING_INSTANCE_DESC* instance_data = nullptr;
            instances->Map(0, nullptr, reinterpret_cast<void**>(&instance_data)) >> kl::verify_result;
            kl::Float4x4 cube = kl::Float4x4::translation({ -1.5f, 2.0f, 2.0f });
            cube *= kl::Float4x4::rotation(kl::Float3(elapsed_t / 2.0f, elapsed_t / 3.0f, elapsed_t / 5.0f) * (float) kl::TO_DEGREES);
            memcpy(&instance_data[0].Transform, &cube, sizeof(float) * 12);

            kl::Float4x4 mirror = kl::Float4x4::translation({ 2.0f, 2.0f, 2.0f });
            mirror *= kl::Float4x4::rotation(kl::Float3(-1.8f, std::sin(elapsed_t) / 8.0f + 1.0f, 0.0f) * (float) kl::TO_DEGREES);
            memcpy(&instance_data[1].Transform, &mirror, sizeof(float) * 12);

            kl::Float4x4 floor = kl::Float4x4::translation({ 0.0f, 0.0f, 2.0f });
            floor *= kl::Float4x4::scaling({ 5.0f, 5.0f, 5.0f });
            memcpy(&instance_data[2].Transform, &floor, sizeof(float) * 12);
            instances->Unmap(0, nullptr);
        }

        // Acceleration structure
        const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC acceleration_descriptor{
            .DestAccelerationStructureData = tlas->GetGPUVirtualAddress(),
            .Inputs{
                .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
                .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE,
                .NumDescs = INSTANCE_COUNT,
                .DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
                .InstanceDescs = instances->GetGPUVirtualAddress(),
            },
            .SourceAccelerationStructureData = tlas->GetGPUVirtualAddress(),
            .ScratchAccelerationStructureData = tlas_update_scratch->GetGPUVirtualAddress(),
        };
        gpu.commands.list->BuildRaytracingAccelerationStructure(&acceleration_descriptor, 0, nullptr);

        // Bind pipeline
        gpu.commands.list->SetPipelineState1(pipeline_state.get());
        gpu.commands.list->SetComputeRootSignature(root_signature.get());
        gpu.commands.list->SetComputeRootDescriptorTable(0, uav_heap->GetGPUDescriptorHandleForHeapStart());
        gpu.commands.list->SetComputeRootShaderResourceView(1, tlas->GetGPUVirtualAddress());
        ID3D12DescriptorHeap* uav_heaps[1] = { uav_heap.get() };
        gpu.commands.list->SetDescriptorHeaps(1, uav_heaps);

        // Dispatch rays
        const D3D12_RESOURCE_DESC target_descriptor = render_target->GetDesc();
        gpu.dispatch_rays(shader_ids->GetGPUVirtualAddress(), (UINT) target_descriptor.Width, (UINT) target_descriptor.Height);

        // Copy to backbuffer
        kl::dx12::Resource back_buffer = gpu.get_back_buffer(gpu.back_buffer_index());
        gpu.commands.transition_resource(render_target, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
        gpu.commands.transition_resource(back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST);
        gpu.commands.copy(back_buffer, render_target);
        gpu.commands.transition_resource(back_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
        gpu.commands.transition_resource(render_target, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

        // Wait and present
        gpu.execute_and_wait();
        gpu.swap_buffers(false);
    }

    // Wait and exit
    gpu.wait();
    return 0;
}
