#pragma once

#include "apis/apis.h"


namespace kl {
    using AudioStorage = std::vector<float>;
}

namespace kl {
    enum class AudioType
    {
        WAV = 0,
        MP3,
    };
}

namespace kl {
    class Audio : public AudioStorage
    {
    public:
        int sample_rate = 0;

        Audio();
        Audio(const std::string_view& path);

        // Alter
        float duration() const;
        void set_duration(float duration);

        void increase_volume(float amount);
		void decrease_volume(float amount);

        // Decoding
        bool load_from_memory(const byte* data, uint64_t byte_size);
        bool load_from_vector(const std::vector<byte>& buffer);
        bool load_from_file(const std::string_view& filepath);

        // Encoding
        bool save_to_vector(std::vector<byte>* buffer, AudioType type) const;
        bool save_to_file(const std::string_view& filepath, AudioType type) const;
    };
}
