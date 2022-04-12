#pragma once

#include <string>
#include <vector>

#include "math/int2.h"
#include "color/color.h"

#pragma comment(lib, "gdiplus.lib")


namespace kl
{
	class image
	{
	private:
		kl::int2 _size;
		std::vector<kl::color> pixels;

	public:
		// Constructor
		image();
		image(const kl::int2& size, const kl::color& color = {});
		image(const std::string& filePath);

		// Getters
		int width() const;
		int height() const;
		kl::int2 size() const;
		kl::color gpixel(const kl::int2& coords) const;
		kl::color* pointer() const;
		kl::image rect(kl::int2 a, kl::int2 b) const;

		// Setters
		void width(int width);
		void height(int height);
		void resize(const kl::int2& size);
		void spixel(const kl::int2& coords, const kl::color& color);

		// Reads an image file and stores it in the image instance
		bool fromFile(const std::string& filePath);

		// Saves the image to a file
		bool toFile(const std::string& fileName) const;

		// Fils the image with solid color
		void fill(const kl::color& color);

		// Returns a x flipped image
		kl::image flipH() const;

		// Returns a y flipped image
		kl::image flipV() const;

		// Draws a line between 2 points
		void drawLine(const kl::int2& a, const kl::int2& b, const kl::color& col);

		// Draws a triangle between 3 points
		void drawTriangle(kl::int2 a, kl::int2 b, kl::int2 c, const kl::color& col, bool fill = false);

		// Draws a rectangle between 2 points
		void drawRectangle(kl::int2 a, kl::int2 b, const kl::color& col, bool fill = false);

		// Draws a circle with the given center point and radius
		void drawCircle(const kl::int2& p, float r, const kl::color& col, bool fill = false);
		// Draws a circle between 1 center and 1 outer point
		void drawCircle(const kl::int2& a, const kl::int2& b, const kl::color& col, bool fill = false);

		// Draws image on image
		void drawImage(const kl::int2& pos, const kl::image& img, bool mixAlpha = true);

		// Converts an image to an ASCII frame
		std::string toASCII(const kl::int2& frameSize) const;
	};
}