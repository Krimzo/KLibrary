#include "klibrary.h"


kl::VideoWriter::VideoWriter(const std::string& filepath, const kl::Int2& frame_size, const int fps, const int bit_rate, const GUID& encoding_format)
    : m_width(frame_size.x), m_height(frame_size.y), m_fps(fps), m_bit_rate(bit_rate), m_encoding_format(encoding_format),
    m_pixel_count(m_width * m_height), m_frame_duration(10'000'000 / m_fps),
    m_frame_byte_width(4 * m_width), m_frame_byte_size(m_frame_byte_width * m_height)
{
    // Sink-Writer
    const std::wstring converted_path = convert_string(filepath);
    MFCreateSinkWriterFromURL(converted_path.c_str(), nullptr, nullptr, &m_sink_writer) >> verify_result;

    // Output media type
    ComPtr<IMFMediaType> media_type_out = nullptr;
    MFCreateMediaType(&media_type_out) >> verify_result;

    // Output media type info
    media_type_out->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video) >> verify_result;
    media_type_out->SetGUID(MF_MT_SUBTYPE, m_encoding_format) >> verify_result;
    media_type_out->SetUINT32(MF_MT_AVG_BITRATE, m_bit_rate) >> verify_result;
    media_type_out->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive) >> verify_result;
    MFSetAttributeSize(media_type_out.Get(), MF_MT_FRAME_SIZE, m_width, m_height) >> verify_result;
    MFSetAttributeRatio(media_type_out.Get(), MF_MT_FRAME_RATE, m_fps, 1) >> verify_result;
    MFSetAttributeRatio(media_type_out.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1) >> verify_result;
    m_sink_writer->AddStream(media_type_out.Get(), &m_stream_index) >> verify_result;

    // Input media type
    ComPtr<IMFMediaType> media_type_in = nullptr;
    MFCreateMediaType(&media_type_in) >> verify_result;

    // Input media type info
    media_type_in->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video) >> verify_result;
    media_type_in->SetGUID(MF_MT_SUBTYPE, m_input_format) >> verify_result;
    media_type_in->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive) >> verify_result;
    MFSetAttributeSize(media_type_in.Get(), MF_MT_FRAME_SIZE, m_width, m_height) >> verify_result;
    MFSetAttributeRatio(media_type_in.Get(), MF_MT_FRAME_RATE, m_fps, 1) >> verify_result;
    MFSetAttributeRatio(media_type_in.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1) >> verify_result;
    m_sink_writer->SetInputMediaType(m_stream_index, media_type_in.Get(), nullptr) >> verify_result;

    // Setup buffers
    MFCreateMemoryBuffer(m_frame_byte_size, &m_media_buffer) >> verify_result;
    m_media_buffer->SetCurrentLength(m_frame_byte_size) >> verify_result;

    MFCreateSample(&m_media_sample) >> verify_result;
    m_media_sample->AddBuffer(m_media_buffer.Get()) >> verify_result;
    m_media_sample->SetSampleDuration((LONGLONG) m_frame_duration) >> verify_result;

    // Start accepting data
    m_sink_writer->BeginWriting() >> verify_result;
}

kl::Int2 kl::VideoWriter::frame_size() const
{
    return { (int) m_width, (int) m_height };
}

int kl::VideoWriter::fps() const
{
    return (int) m_fps;
}

int kl::VideoWriter::bit_rate() const
{
    return (int) m_bit_rate;
}

GUID kl::VideoWriter::format() const
{
    return m_encoding_format;
}

bool kl::VideoWriter::add_frame(const Image& frame)
{
    const Int2 frame_size = this->frame_size();
    if (frame.width() != frame_size.x || frame.height() != frame_size.y) {
        return false;
    }

    const Color* in_buffer = frame;
    Color* out_buffer = nullptr;

    m_media_buffer->Lock((BYTE**) &out_buffer, nullptr, nullptr) >> verify_result;
    for (uint32_t y = 0; y < m_height; y++) {
        memcpy(out_buffer + (m_height - 1 - y) * m_width, in_buffer + y * m_width, m_frame_byte_width);
    }
    m_media_buffer->Unlock() >> verify_result;

    if (FAILED(m_media_sample->SetSampleTime((LONGLONG) duration_100ns()))) {
        return false;
    }
    if (FAILED(m_sink_writer->WriteSample(m_stream_index, m_media_sample.Get()))) {
        return false;
    }

    m_frame_counter += 1;
    return true;
}

int kl::VideoWriter::frame_count() const
{
    return m_frame_counter;
}

uint64_t kl::VideoWriter::duration_100ns() const
{
    return m_frame_counter * m_frame_duration;
}

float kl::VideoWriter::duration_seconds() const
{
    static constexpr float diver = 1.0f / 1e7f;
    return duration_100ns() * diver;
}

bool kl::VideoWriter::finalize() const
{
    return SUCCEEDED(m_sink_writer->Finalize());
}
