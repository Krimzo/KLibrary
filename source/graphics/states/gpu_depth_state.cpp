#include "graphics/states/gpu_depth_state.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_depth_state> kl::gpu_depth_state::make(const dx::depth_state_descriptor* descriptor)
{
    return ref<gpu_depth_state>(new gpu_depth_state(descriptor));
}

kl::ref<kl::gpu_depth_state> kl::gpu_depth_state::make(bool depth, bool stencil, bool mask)
{
    return ref<gpu_depth_state>(new gpu_depth_state(depth, stencil, mask));
}

// Class
kl::gpu_depth_state::gpu_depth_state(const dx::depth_state_descriptor* descriptor)
{
    const long result = BOUND_GPU->device()->CreateDepthStencilState(descriptor, &child_object_);
    warning_check(!child_object_, format("Failed to create depth stencil state. Result: 0x", std::hex, result));
}

kl::gpu_depth_state::gpu_depth_state(const bool depth, const bool stencil, const bool mask)
{
    dx::depth_state_descriptor descriptor = {};

    if (depth) {
        descriptor.DepthEnable = true;
        descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        descriptor.DepthFunc = D3D11_COMPARISON_LESS;
    }

    if (stencil) {
        descriptor.StencilEnable = true;
        descriptor.StencilReadMask = 0xFF;
        descriptor.StencilWriteMask = 0xFF;

        if (mask) {
            descriptor.DepthEnable = false;
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
        }
        else {
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        }
    }

    this->gpu_depth_state::gpu_depth_state(&descriptor);
}

void kl::gpu_depth_state::bind() const
{
    bind(0xFF);
}

void kl::gpu_depth_state::bind(const UINT stencil_ref) const
{
    BOUND_GPU->context()->OMSetDepthStencilState(child_object_, stencil_ref);
}

void kl::gpu_depth_state::unbind() const
{
    BOUND_GPU->context()->OMSetDepthStencilState(nullptr, NULL);
}
