#include "klibrary.h"

void kl::Entity::update_physics(float delta_t)
{
    velocity += acceleration * delta_t;
    position += velocity * delta_t;
    rotation += angular * delta_t;
}

kl::float4x4 kl::Entity::matrix() const
{
    return float4x4::translation(position) * float4x4::rotation(rotation) * float4x4::scaling(scale);
}
