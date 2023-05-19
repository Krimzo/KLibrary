#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T>
    class array
    {
        T* m_data = nullptr;
        uint64_t* m_count = nullptr;
        uint64_t m_size = 0;

    public:
        // Create
        array()
        {}

        array(const uint64_t size)
            : m_size(size)
        {
            // Allocate array data
            m_data = new T[size]();
            if (!m_data) throw std::runtime_error("Could not allocate memory for array data.");

            // Allocate counter
            m_count = new uint64_t;
            if (!m_count) throw std::runtime_error("Could not allocate memory for reference counter.");
            *m_count = 1;
        }

        // Destroy
        virtual ~array()
        {
            this->free();
        }

        void free()
        {
            if (m_count && !(--(*m_count))) {
                delete[] m_data;
                delete m_count;
            }
            m_data = nullptr;
            m_count = nullptr;
            m_size = 0;
        }

        // Create copy
        array(const array<T>& other)
        {
            m_data = other.m_data;
            m_count = other.m_count;
            m_size = other.m_size;
            if (m_count) *m_count += 1;
        }

        array(const array<T>&& other)
            : array(other)
        {}

        // Copy
        array& operator=(const array<T>& other)
        {
            // Address check
            if (other.m_data == m_data) {
                return *this;
            }

            // Do copy
            this->free();
            m_data = other.m_data;
            m_count = other.m_count;
            m_size = other.m_size;
            if (m_count) *m_count += 1;
            return *this;
        }

        array& operator=(const array<T>&& other)
        {
            return (*this = other);
        }

        // Info
        operator bool() const
        {
            return (bool) m_data;
        }

        uint64_t count() const
        {
            return m_count ? *m_count : 0;
        }

        uint64_t size() const
        {
            return m_size;
        }

        bool empty() const
        {
            return size() == 0;
        }

        // Address
        T* operator&()
        {
            return m_data;
        }

        const T* operator&() const
        {
            return m_data;
        }

        // Access
        T& operator*()
        {
            return *m_data;
        }

        const T& operator*() const
        {
            return *m_data;
        }

        T* operator->()
        {
            return m_data;
        }

        const T* operator->() const
        {
            return m_data;
        }

        T& operator[](const uint64_t index)
        {
            return m_data[index];
        }

        const T& operator[](const uint64_t index) const
        {
            return m_data[index];
        }
    };
}

namespace kl {
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const array<T>& array)
    {
        stream << "(" << array.count() << " | 0x" << std::hex << &array << std::dec << " | ";
        if (array.empty()) {
            stream << "[]";
        }
        else {
            stream << "[";
            for (uint64_t i = 0; i < array.size() - 1; i++) {
                stream << array[i] << ", ";
            }
            stream << array[array.size() - 1] << "]";
        }
        stream << ")";
        return stream;
    }
}
