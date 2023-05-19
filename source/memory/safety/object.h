#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T>
    class object
    {
        T* m_instance = nullptr;
        uint64_t* m_count = nullptr;

    public:
        // Create
        object()
        {}

        object(T* instance)
            : m_instance(instance)
        {
            // Instance check
            if (!m_instance) {
                return;
            }

            // Allocate counter
            m_count = new uint64_t;
            if (!m_count) throw std::runtime_error("Could not allocate memory for reference counter.");
            *m_count = 1;
        }

        // Destroy
        virtual ~object()
        {
            this->free();
        }

        void free()
        {
            if (m_count && !(--(*m_count))) {
                delete m_instance;
                delete m_count;
            }
            m_instance = nullptr;
            m_count = nullptr;
        }

        // Create copy
        object(const object<T>& other)
        {
            m_instance = other.m_instance;
            m_count = other.m_count;
            if (m_count) *m_count += 1;
        }

        object(const object<T>&& other)
            : object(other)
        {}

        // Copy
        object& operator=(const object<T>& other)
        {
            // Address check
            if (&other == m_instance) {
                return *this;
            }

            // Do copy
            this->free();
            m_instance = other.m_instance;
            m_count = other.m_count;
            if (m_count) *m_count += 1;
            return *this;
        }

        object& operator=(const object<T>&& other)
        {
            return (*this = other);
        }

        // Info
        operator bool() const
        {
            return (bool) m_instance;
        }

        uint64_t count() const
        {
            return m_count ? *m_count : 0;
        }

        // Address
        T* operator&()
        {
            return m_instance;
        }

        const T* operator&() const
        {
            return m_instance;
        }

        // Access
        T& operator*()
        {
            return *m_instance;
        }

        const T& operator*() const
        {
            return *m_instance;
        }

        T* operator->()
        {
            return m_instance;
        }

        const T* operator->() const
        {
            return m_instance;
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const object<T>& object)
    {
        stream << "(" << object.count() << " | 0x" << std::hex << &object << std::dec << " | ";
        if (object) {
            stream << *object;
        }
        else {
            stream << "()";
        }
        stream << ")";
        return stream;
    }
}
