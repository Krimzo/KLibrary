#include "graphics/video.h"

#include "utility/console.h"
#include "utility/strings.h"


namespace kl {
    static int video_instance_count = 0;
    static bool utility_initialised = false;
    static std::mutex thread_lock = {};

    static void init_utility()
    {
        thread_lock.lock();
        video_instance_count++;
        if (!utility_initialised) {
            assert(FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY)) ||
                    FAILED(MFStartup(MF_VERSION)), "Failed to initialize video utility");
            utility_initialised = true;
        }
        thread_lock.unlock();
    }

    static void free_utility()
    {
        thread_lock.lock();
        video_instance_count--;
        if (utility_initialised && video_instance_count == 0) {
            MFShutdown();
            CoUninitialize();
            utility_initialised = false;
        }
        thread_lock.unlock();
    }

    static void configure_decoder(IMFSourceReader* reader)
    {
        IMFMediaType* type = nullptr;
        assert(FAILED(reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &type)),
            "Failed to get default video type");

        GUID major_type = {};
        assert(FAILED(type->GetGUID(MF_MT_MAJOR_TYPE, &major_type)), "Failed to get major video type");

        IMFMediaType* new_type = nullptr;
        assert(FAILED(MFCreateMediaType(&new_type)), "Failed to create new video type");
        assert(FAILED(new_type->SetGUID(MF_MT_MAJOR_TYPE, major_type)), "Failed to set major video type");
        assert(FAILED(new_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32)), "Failed to set sub video type");
        assert(FAILED(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, new_type)),
            "Failed to set video type");

        type->Release();
        new_type->Release();
    }
}

kl::video::video()
{
    init_utility();
}

kl::video::video(const std::string& filepath) : video()
{
    open(filepath);
}

kl::video::~video()
{
    close();
    free_utility();
}

bool kl::video::is_open() const
{
    return static_cast<bool>(reader_);
}

void kl::video::open(const std::string& filepath)
{
    close();
    IMFAttributes* attr = nullptr;
    MFCreateAttributes(&attr, 1);
    attr->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, true);

    const std::wstring w_file_path = strings::to_w_string(filepath);
    MFCreateSourceReaderFromURL(w_file_path.c_str(), attr, &reader_);
    if (reader_) {
        configure_decoder(reader_);
    }

    attr->Release();
}

void kl::video::close()
{
    if (reader_) {
        reader_->Release();
        reader_ = nullptr;
    }
}

uint64_t kl::video::size() const
{
    if (reader_) {
        PROPVARIANT var = {};
        if (SUCCEEDED(reader_->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_TOTAL_FILE_SIZE, &var))) {
            uint64_t byte_size = 0;
            PropVariantToUInt64(var, &byte_size);
            PropVariantClear(&var);
            return byte_size;
        }
    }
    return 0;
}

float kl::video::duration() const
{
    if (reader_) {
        PROPVARIANT var = {};
        if (SUCCEEDED(reader_->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var))) {
            int64_t time = 0;
            PropVariantToInt64(var, &time);
            PropVariantClear(&var);
            return static_cast<float>(time) * 1e-7f;
        }
    }
    return 0.0f;
}

float kl::video::fps() const
{
    if (reader_) {
        IMFMediaType* current_type = nullptr;
        reader_->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
        if (current_type) {
            uint32_t val1 = 0, val2 = 0;
            MFGetAttributeRatio(current_type, MF_MT_FRAME_RATE, &val1, &val2);
            current_type->Release();
            return static_cast<float>(val1) / static_cast<float>(val2);
        }
    }
    return 0.0f;
}

uint32_t kl::video::frame_count() const
{
    return static_cast<uint32_t>(fps() * duration());
}

kl::uint2 kl::video::frame_size() const
{
    if (reader_) {
        IMFMediaType* current_type = nullptr;
        reader_->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current_type);
        if (current_type) {
            uint2 frame_size = {};
            MFGetAttributeSize(current_type, MF_MT_FRAME_SIZE, &frame_size.x, &frame_size.y);
            current_type->Release();
            return frame_size;
        }
    }
    return {};
}

bool kl::video::get_next_frame(image& out) const
{
    bool state = false;
    if (reader_) {
        DWORD flags = 0;
        int64_t time_stamp = 0;
        IMFSample* sample = nullptr;
        reader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &time_stamp, &sample);

        if (sample) {
            IMFMediaBuffer* media_buffer = nullptr;
            sample->ConvertToContiguousBuffer(&media_buffer);

            if (media_buffer) {
                DWORD data_length = 0;
                uint8_t* frame_data = nullptr;
                media_buffer->Lock(&frame_data, nullptr, &data_length);

                out.set_size(frame_size());
                memcpy(out.data(), frame_data, data_length);

                media_buffer->Release();
                state = true;
            }
            sample->Release();
        }
    }
    return state;
}
