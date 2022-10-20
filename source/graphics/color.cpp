#include "graphics/color.h"

#include "math/math.h"


kl::color::color()
    : b(0), g(0), r(0), a(255)
{}

kl::color::color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    : b(b), g(g), r(r), a(a)
{}

bool kl::color::operator==(const color& obj) const
{
    return r == obj.r && g == obj.g && b == obj.b && a == obj.a;
}

bool kl::color::operator!=(const color& obj) const
{
    return !(*this == obj);
}

kl::color kl::color::gray() const
{
    const auto gray_value = static_cast<uint8_t>(static_cast<float>(r) * 0.3f + static_cast<float>(g) * 0.585f +
        static_cast<float>(b) * 0.115f);
    return {gray_value, gray_value, gray_value, a};
}

kl::color kl::color::inverted() const
{
    return {static_cast<uint8_t>(255 - r), static_cast<uint8_t>(255 - g), static_cast<uint8_t>(255 - b), a};
}

char kl::color::as_ascii() const
{
    static constexpr char ascii_table[10] = {'@', '%', '#', 'x', '+', '=', ':', '-', '.', ' '};
    static constexpr float conversion = 9.0f / 255.0f;
    return ascii_table[static_cast<int>(static_cast<float>(gray().r) * conversion)];
}

kl::color kl::color::mix(const color& color, float ratio) const
{
    ratio = math::minmax(ratio, 0, 1);
    return {
        static_cast<uint8_t>(static_cast<uint8_t>(static_cast<float>(r) * (1.0f - ratio)) + static_cast<uint8_t>(
            static_cast<float>(color.r) * ratio)),
        static_cast<uint8_t>(static_cast<uint8_t>(static_cast<float>(g) * (1.0f - ratio)) + static_cast<uint8_t>(
            static_cast<float>(color.g) * ratio)),
        static_cast<uint8_t>(static_cast<uint8_t>(static_cast<float>(b) * (1.0f - ratio)) + static_cast<uint8_t>(
            static_cast<float>(color.b) * ratio))
    };
}

kl::color kl::color::mix(const color& color) const
{
    return mix(color, to_float_color(color.a));
}

std::ostream& kl::operator<<(std::ostream& stream, const color& object)
{
    stream << "\033[38;2;" << static_cast<int>(object.r) << ";" << static_cast<int>(object.g) << ";" << static_cast<int>(object.b) <<
        "m";
    return stream;
}
