#pragma once

#include "apis/apis.h"


namespace kl {
    class gpu;
}

namespace kl {
    template<typename T>
    struct shader_holder
    {
        gpu* gpu = nullptr;
        T shader = nullptr;
        dx::buffer cbuffer = nullptr;

        // Construct
        shader_holder(kl::gpu* gpu);
        shader_holder(kl::gpu* gpu, const T& shader);
        ~shader_holder();

        // Get
        operator T() const;
        operator bool() const;

        // Update
        template<typename O>
        void update_cbuffer(const O& object)
        {
            update_cbuffer(&object, sizeof(O));
        }
     
        void update_cbuffer(const void* data, UINT byte_size);
    };
}
