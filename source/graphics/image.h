#pragma once

#include <string>
#include <vector>

#include "graphics/color.h"
#include "math/math.h"


namespace kl
{
	class image
	{
	private:
		kl::uint2 m_Size;
		std::vector<kl::color> m_Pixels;

	public:
		image();
		image(const kl::uint2& size, const kl::color& color = {});
		image(const std::string& filePath);

		std::vector<kl::color>::iterator begin();
		std::vector<kl::color>::iterator end();

		uint width() const;
		uint height() const;
		kl::uint2 size() const;
		kl::color pixel(const kl::uint2& coords) const;
		kl::color* data();
		const kl::color* data() const;
		kl::image rect(kl::uint2 a, kl::uint2 b) const;

		bool width(uint width);
		bool height(uint height);
		bool resize(const kl::uint2& size);
		bool pixel(const kl::uint2& coords, const kl::color& color);

		bool fromFile(const std::string& filePath);
		bool toFile(const std::string& fileName) const;

		kl::image flipH() const;
		kl::image flipV() const;

		void fill(const kl::color& color);
		void drawLine(const kl::int2& a, const kl::int2& b, const kl::color& col);
		void drawTriangle(kl::int2 a, kl::int2 b, kl::int2 c, const kl::color& col, bool fill = false);
		void drawRectangle(kl::int2 a, kl::int2 b, const kl::color& col, bool fill = false);
		void drawCircle(const kl::int2& p, float r, const kl::color& col, bool fill = false);
		void drawCircle(const kl::int2& a, const kl::int2& b, const kl::color& col, bool fill = false);
		void drawImage(const kl::int2& pos, const kl::image& img, bool mixAlpha = true);

		std::string ascii(const kl::int2& frameSize) const;
	};
}