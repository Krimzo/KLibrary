#include "examples.h"


/* VISUAL SORT */
class Stick
{
public:
    int value = 0;
    kl::Color color = {};
};

static std::vector<Stick> generate_sticks(const int count, const int min_value_incl, const int max_value_excl)
{
    int stored_min_value = max_value_excl;
    int stored_max_value = min_value_incl;

    std::vector<Stick> sticks(count);
    for (auto& [value, color] : sticks) {
        value = kl::random::gen_int(min_value_incl, max_value_excl);
        stored_min_value = std::min(stored_min_value, value);
        stored_max_value = std::max(stored_max_value, value);
    }

    for (auto& [value, color] : sticks) {
        const byte gray_value = (byte) (kl::wrap((float) value, (float) stored_min_value, (float) stored_max_value) * 255.0f);
        color = { gray_value, gray_value, gray_value };
    }

    return sticks;
}

static void draw_sticks(kl::Image& frame, const std::vector<Stick>& sticks)
{
    for (int i = 0; i < (int) sticks.size(); i++) {
        frame.draw_line({ i, frame.height() - 1 }, { i, frame.height() - 1 - sticks[i].value }, sticks[i].color);
    }
}
/* ----------- */

static const std::string OUTPUT_VIDEO_PATH = "./media/generated_video.mp4";

int examples::video_writing_main()
{
    auto video_writer = kl::VideoWriter(OUTPUT_VIDEO_PATH, { 1920, 1080 }, 60, 80'000'000, MFVideoFormat_H264);

    kl::Image frame = { video_writer.frame_size() };
    std::vector<Stick> sticks = generate_sticks(frame.width(), 1, frame.height());

    for (size_t i = 0; i < sticks.size() - 1; i++) {
        for (size_t j = i + 1; j < sticks.size(); j++) {
            if (sticks[j].value < sticks[i].value) {
                std::swap(sticks[i], sticks[j]);
            }
        }

        frame.fill(kl::colors::GRAY);
        draw_sticks(frame, sticks);
        video_writer.add_frame(frame);

        kl::print("[", i + 1, "/", sticks.size(), "]");
    }

    video_writer.finalize();
    return 0;
}
