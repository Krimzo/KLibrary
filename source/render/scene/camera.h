#pragma once

#include "render/components/texture.h"
#include "memory/memory.h"
#include "math/math.h"

namespace kl
{
struct Camera
{
    float3 position;

    float aspect_ratio = 1.7778f;
    float field_of_view = 90.0f;

    float near_plane = 0.01f;
    float far_plane = 500.0f;

    float sensitivity = 0.1f;
    float speed = 2.0f;

    RGB background;
    Ref<Texture> skybox;

    void update_aspect_ratio(int2 size);
    Ray ray() const;

    void set_h_fov(float fov);
    void set_v_fov(float fov);

    void set_forward(float3 const& dir);
    void set_up(float3 const& dir);

    float3 forward() const;
    float3 right() const;
    float3 up() const;

    void move_forward(float delta_time);
    void move_back(float delta_time);
    void move_right(float delta_time);
    void move_left(float delta_time);
    void move_up(float delta_time);
    void move_down(float delta_time);

    void rotate(float2 mouse_pos, float2 frame_center, float vertical_angle_limit = 85.0f);

    float4x4 view_matrix() const;
    float4x4 projection_matrix() const;
    float4x4 matrix() const;

  private:
    float3 m_forward = {0.0f, 0.0f, 1.0f};
    float3 m_up = {0.0f, 1.0f, 0.0f};
};
} // namespace kl
