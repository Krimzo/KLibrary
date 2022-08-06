#pragma once

#include <string>
#include <vector>

#include "Utility/Strings.h"
#include "Math/Math.h"


namespace kl {
	class Image {
		UInt2 m_Size;
		Vector<kl::Color> m_Pixels;

	public:
		Image();
		Image(const UInt2& size, const Color& color = {});
		Image(const String& filePath);

		Vector<Color>::iterator begin();
		Vector<Color>::iterator end();

		uint getWidth() const;
		void setWidth(uint width);

		uint getHeight() const;
		void setHeight(uint height);

		UInt2 getSize() const;
		void setSize(const UInt2& size);

		Color getPixel(const UInt2& coords) const;
		void setPixel(const UInt2& coords, const Color& color);

		Color* data();
		const Color* data() const;

		Image getPart(UInt2 a, UInt2 b) const;

		bool loadFromFile(const String& filePath);
		bool saveToFile(const String& fileName) const;

		kl::Image flip(bool vertical) const;

		void fill(const kl::Color& color);

		void drawLine(const kl::Int2& a, const kl::Int2& b, const kl::Color& color);

		void drawTriangle(kl::Int2 a, kl::Int2 b, kl::Int2 c, const kl::Color& color, bool fill = false);

		void drawRectangle(kl::Int2 a, kl::Int2 b, const kl::Color& color, bool fill = false);

		void drawCircle(const kl::Int2& p, float r, const kl::Color& color, bool fill = false);
		void drawCircle(const kl::Int2& a, const kl::Int2& b, const kl::Color& color, bool fill = false);

		void drawImage(const kl::Int2& position, const kl::Image& image, bool mixAlpha = true);

		String asASCII(const kl::Int2& frameSize) const;
	};
}