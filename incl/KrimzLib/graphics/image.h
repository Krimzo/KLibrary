#pragma once

#include <string>
#include <vector>

#include "KrimzLib/math/int2.h"
#include "KrimzLib/graphics/color.h"

#pragma comment(lib, "gdiplus.lib")


namespace kl {
	class image {
	private:
		int width;
		int height;
		std::vector<kl::color> pixels;

	public:
		// Constructor
		image();
		image(const kl::int2& size, const kl::color& color = {});
		image(const char* fileName);

		// Getters
		int getWidth() const;
		int getHeight() const;
		kl::int2 getSize() const;
		kl::color getPixel(const kl::int2& point) const;
		int pixelCount() const;
		byte* pointer() const;
		kl::image getRect(kl::int2 a, kl::int2 b) const;

		// Setters
		void setWidth(int width);
		void setHeight(int height);
		void setSize(const kl::int2& size);
		void setPixel(const kl::int2& point, const kl::color& color);

		// Reads an image file and stores it in the image instance
		void fromFile(const std::string& filePath);

		// Saves the image to a file
		void toFile(const std::string& fileName) const;

		// Fils the image with solid color
		void fill(const kl::color& color);

		// Returns a x flipped image
		kl::image flipHorizontal() const;

		// Returns a y flipped image
		kl::image flipVertical() const;

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

		// Converts an image to an ASCII frame
		std::string toASCII(const kl::int2& frameSize);
	};
}