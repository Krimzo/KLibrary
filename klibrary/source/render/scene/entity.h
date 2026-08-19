#pragma once

#include "render/components/mesh.h"
#include "render/components/material.h"


namespace kl
{
struct Entity
{
    float3 scale{ 1.0f };
    float3 rotation;
    float3 position;

    float3 acceleration;
    float3 velocity;
    float3 angular;

    Ref<Mesh> mesh;
    Ref<Material> material;

    void update_physics( float delta_t );

    float4x4 matrix() const;
};
}
