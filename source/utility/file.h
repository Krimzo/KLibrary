#pragma once

#include "render/vertex.h"

#include <filesystem>
#include <string>
#include <vector>


namespace kl::files {
    std::string get_extension(const std::string& filepath);

    std::vector<std::string> get_files(const std::string& path, bool recursive = false);

    std::string read_string(const std::string& filepath);

    bool write_string(const std::string& filepath, const std::string& data);

    bool append_string(const std::string& filepath, const std::string& data, int position = -1);

    std::vector<vertex> parse_mesh(const std::string& filepath, bool flip_z = true);
}

namespace kl {
    class file
    {
        FILE* file_ = nullptr;

    public:
        file();

        explicit file(const std::string& filepath, bool clear = true);

        file(const file&) = delete;

        void operator=(const file&) = delete;

        ~file();

        explicit operator bool() const;

        [[nodiscard]] bool is_open() const;

        bool open(const std::string& filepath, bool clear = true);

        bool close();

        bool seek(int64_t position) const;

        bool move(int64_t delta) const;

        bool rewind() const;

        bool unwind() const;

        [[nodiscard]] int64_t tell() const;

        template <typename T>
        uint64_t read(T& object) const
        {
            return read(&object, 1);
        }

        template <typename T>
        uint64_t read(T* buffer, const uint64_t count) const
        {
            if (file_) {
                return fread(buffer, sizeof(T), count, file_);
            }
            return 0;
        }

        template <typename T>
        uint64_t write(const T& object)
        {
            return write(&object, 1);
        }

        template <typename T>
        uint64_t write(const T* buffer, const uint64_t count)
        {
            if (file_) {
                return fwrite(buffer, sizeof(T), count, file_);
            }
            return 0;
        }
    };
}
