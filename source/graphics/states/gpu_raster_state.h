#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_raster_state : public gpu_child<dx::raster_state>
    {
        gpu_raster_state(const dx::raster_state_descriptor* descriptor);
        gpu_raster_state(bool wireframe, bool cull, bool cull_back);

    public:
        static ref<gpu_raster_state> make(const dx::raster_state_descriptor* descriptor);
        static ref<gpu_raster_state> make(bool wireframe, bool cull, bool cull_back);

        void bind() const;
        void unbind() const;
    };
}
