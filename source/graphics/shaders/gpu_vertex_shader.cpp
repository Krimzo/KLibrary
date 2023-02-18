#include "graphics/shaders/gpu_vertex_shader.h"

#include "graphics/buffers/gpu_shader_data.h"
#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_vertex_shader> kl::gpu_vertex_shader::make(const std::string& source, const std::vector<dx::layout_descriptor>& descriptors)
{
    return ref<gpu_vertex_shader>(new gpu_vertex_shader(source, descriptors));
}

// Class
kl::gpu_vertex_shader::gpu_vertex_shader(const std::string& source, const std::vector<dx::layout_descriptor>& descriptors)
{
    static const dx::layout_descriptor default_layout_descriptors[3] = {
        { "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "KL_Texture", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    const auto shader_data = gpu_shader_data::make('v', source);
    BOUND_GPU->device()->CreateVertexShader(shader_data->get_data(), shader_data->data_size(), nullptr, &child_object_);
    warning_check(!child_object_, "Failed to create vertex shader");

    BOUND_GPU->device()->CreateInputLayout(!descriptors.empty() ? descriptors.data() : default_layout_descriptors,
        !descriptors.empty() ? (UINT) descriptors.size() : 3,
        shader_data->get_data(), shader_data->data_size(), &layout_);
    warning_check(!layout_, "Failed to create input layout");
}

kl::gpu_vertex_shader::~gpu_vertex_shader()
{
    safe_release(layout_);
}

void kl::gpu_vertex_shader::bind() const
{
    BOUND_GPU->context()->VSSetShader(child_object_, nullptr, 0);
    BOUND_GPU->context()->IASetInputLayout(layout_);
}

void kl::gpu_vertex_shader::unbind() const
{
    BOUND_GPU->context()->VSSetShader(nullptr, nullptr, 0);
    BOUND_GPU->context()->IASetInputLayout(nullptr);
}
