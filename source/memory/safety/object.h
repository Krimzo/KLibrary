#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T>
    class object
    {
        T* m_instance = nullptr;
        uint64_t* m_count = nullptr;

        template<typename>
        friend class object;

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
        template<typename O> requires std::is_base_of<T, O>::value
        object(const object<O>& other)
        {
            m_instance = other.m_instance;
            m_count = other.m_count;
            if (m_count) *m_count += 1;
        }

        template<typename O> requires std::is_base_of<T, O>::value
        object(const object<O>&& other) noexcept
            : object<T>(other)
        {}

        // Copy
        template<typename O> requires std::is_base_of<T, O>::value
        object<T>& operator=(const object<O>& other)
        {
            // Address check
            if (other.m_instance == m_instance) {
                return *this;
            }

            // Do copy
            this->free();
            m_instance = other.m_instance;
            m_count = other.m_count;
            if (m_count) *m_count += 1;
            return *this;
        }

        template<typename O> requires std::is_base_of<T, O>::value
        object<T>& operator=(const object<O>&& other) noexcept
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

        // Compare
        template<typename O>
        bool operator==(const object<O>& other) const
        {
            const void* first = m_instance;
            const void* second = other.m_instance;
            return first == second;
        }

        template<typename O>
        bool operator!=(const object<O>& other) const
        {
            return !(*this == other);
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
