#include "klib.h"


/* VISUAL SORT */
struct stick
{
    int value = 0;
    kl::color color = {};
};

static std::vector<stick> generate_sticks(const int count, const int min_value_incl, const int max_value_excl)
{
    int stored_min_value = max_value_excl;
    int stored_max_value = min_value_incl;

    std::vector<stick> sticks(count);
    for (auto& [value, color] : sticks) {
        value = kl::random::get_int(min_value_incl, max_value_excl);
        stored_min_value = min(stored_min_value, value);
        stored_max_value = max(stored_max_value, value);
    }

    for (auto& [value, color] : sticks) {
        const uint8_t gray_value = (byte) (kl::math::normalize((float) value, (float) stored_min_value, (float) stored_max_value) * 255.0f);
        color = { gray_value, gray_value, gray_value };
    }

    return sticks;
}

static void draw_sticks(kl::image& frame, const std::vector<stick>& sticks)
{
    for (int i = 0; i < int(sticks.size()); i++) {
        frame.draw_line({ i, frame.height() - 1 }, { i, frame.height() - 1 - sticks[i].value }, sticks[i].color);
    }
}
/* ----------- */

static const std::string output_video_path = "examples/videos/generated_video.mp4";

int main()
{
    auto video_writer = kl::video_writer(output_video_path, { 1920, 1080 }, 60, 80'000'000, MFVideoFormat_H264);

    kl::image frame = { video_writer.frame_size() };
    std::vector<stick> sticks = generate_sticks(frame.width(), 1, frame.height());

    for (size_t i = 0; i < sticks.size() - 1; i++) {
        for (size_t j = i + 1; j < sticks.size(); j++) {
            if (sticks[j].value < sticks[i].value) {
                std::swap(sticks[i], sticks[j]);
            }
        }

        frame.fill(kl::colors::gray);
        draw_sticks(frame, sticks);
        video_writer.add_frame(frame);

        kl::print("[", i + 1, "/", sticks.size(), "]");
    }

    video_writer.finalize();
}
