#pragma once

#include "memory/reference/ref.h"


namespace kl {
    void safe_release(IUnknown*& unknown);
    void safe_release(IUnknown* const& unknown);

    template<typename T>
    class gpu_child
    {
    protected:
        T child_object_ = nullptr;

    public:
        gpu_child()
        {}

        virtual ~gpu_child()
        {
            safe_release(child_object_);
        }

        gpu_child(const gpu_child&) = delete;
        gpu_child(const gpu_child&&) = delete;

        void operator=(const gpu_child&) = delete;
        void operator=(const gpu_child&&) = delete;

        operator T() const
        {
            return child_object_;
        }

        operator T* ()
        {
            return (child_object_ ? &child_object_ : nullptr);
        }

        operator bool() const
        {
            return (bool) child_object_;
        }
    };
}
