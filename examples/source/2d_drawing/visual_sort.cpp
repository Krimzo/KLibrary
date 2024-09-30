#include "examples.h"


struct Stick
{
    int value = 0;
    kl::RGB color;
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
        const byte gray_value = (byte) (kl::unlerp((float) value, (float) stored_min_value, (float) stored_max_value) * 255.0f);
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

int examples::visual_sort_main(const int argc, const char** argv)
{
    kl::Window window{ "Visual Sort" };
    kl::Image frame{ window.size() };

    std::vector<Stick> sticks = generate_sticks(frame.width(), 1, frame.height());

    std::thread([&]
    {
        window.set_title("Sorting...");
        for (size_t i = 0; i < sticks.size() - 1; i++) {
            for (size_t j = i + 1; j < sticks.size(); j++) {
                if (sticks[j].value < sticks[i].value) {
                    std::swap(sticks[i], sticks[j]);
                }
                kl::time::wait(0.000005f);
            }
        }
        window.set_title("Finished!");
    }).detach();

    while (window.process()) {
        frame.fill(kl::colors::GRAY);
        draw_sticks(frame, sticks);
        window.draw_image(frame);
    }
    return 0;
}
