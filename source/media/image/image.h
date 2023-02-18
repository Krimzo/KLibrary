#pragma once

#include "math/math.h"


namespace kl {
    class image
    {
        std::vector<color> pixels_;
        int2 size_ = {};

    public:
        static image get_screenshot();

        image() = default;
        image(const int2& size, const color& color = {});
        explicit image(const std::string& filepath);

        std::vector<color>::iterator begin();
        std::vector<color>::iterator end();

        int width() const;

        void set_width(int width);

        int height() const;

        void set_height(int height);

        int2 size() const;

        void set_size(const int2& size, bool scale = false);

        color get_pixel(const int2& coords) const;

        void set_pixel(const int2& coords, const color& color);

        color& operator[](const int2& coords);
        const color& operator[](const int2& coords) const;

        color* data();

        const color* data() const;

        image get_rectangle(int2 top_left, int2 bottom_right) const;

        bool load_from_file(const std::string& filepath);

        bool save_to_file(const std::string& filepath) const;

        image flip(bool vertical) const;

        void fill(const color& color);

        void draw_line(const int2& from, const int2& to, const color& color);

        void draw_triangle(int2 position_a, int2 position_b, int2 position_c, const color& color, bool fill = false);

        void draw_rectangle(int2 top_left, int2 bottom_right, const color& color, bool fill = false);

        void draw_circle(const int2& center, float radius, const color& color, bool fill = false);

        void draw_circle(const int2& center, const int2& outer_position, const color& color, bool fill = false);

        void draw_image(const int2& position, const image& image, bool mix_alpha = true);

        std::string as_ascii(const int2& frame_size) const;
    };
}
