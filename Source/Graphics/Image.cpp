#include "graphics/image.h"

#include <iostream>
#include <sstream>
#include <windows.h>
#include <gdiplus.h>

#include "math/math.h"
#include "utility/file.h"
#include "utility/strings.h"
#include "utility/console.h"

#undef min
#undef max

#pragma comment(lib, "gdiplus.lib")


kl::Image::Image() {
	setSize({});
}

kl::Image::Image(const UInt2& size, const Color& color) {
	setSize(size);
	fill(color);
}

kl::Image::Image(const String& filePath) {
	if (!loadFromFile(filePath)) {
		this->kl::Image::Image();
	}
}

kl::Vector<kl::Color>::iterator kl::Image::begin() {
	return m_Pixels.begin();
}

kl::Vector<kl::Color>::iterator kl::Image::end() {
	return m_Pixels.end();
}

kl::uint kl::Image::getWidth() const {
	return m_Size.x;
}

void kl::Image::setWidth(uint width) {
	setSize(kl::Int2(width, m_Size.y));
}

kl::uint kl::Image::getHeight() const {
	return m_Size.y;
}

void kl::Image::setHeight(uint height) {
	setSize(kl::Int2(m_Size.x, height));
}

kl::UInt2 kl::Image::getSize() const {
	return m_Size;
}

void kl::Image::setSize(const kl::UInt2& size) {
	if (size != m_Size) {
		m_Pixels.resize(uint64(size.x) * size.y);
		m_Size = size;
	}
}

kl::Color kl::Image::getPixel(const kl::UInt2& coords) const {
	if (coords.x < m_Size.x && coords.y < m_Size.y) {
		return m_Pixels[uint64(coords.y) * m_Size.x + coords.x];
	}
	return Colors::Black;
}

void kl::Image::setPixel(const kl::UInt2& coords, const kl::Color& color) {
	if (coords.x < m_Size.x && coords.y < m_Size.y) {
		m_Pixels[uint64(coords.y) * m_Size.x + coords.x] = color;
	}
}

kl::Color* kl::Image::data() {
	return m_Pixels.data();
}

const kl::Color* kl::Image::data() const {
	return m_Pixels.data();
}

kl::Image kl::Image::getPart(UInt2 a, UInt2 b) const {
	if (b.x < a.x) {
		std::swap(a.x, b.x);
	}
	if (b.y < a.y) {
		std::swap(a.y, b.y);
	}

	Image temp(b - a);
	for (UInt2 position; position.y < temp.getHeight(); position.y++) {
		for (position.x = 0; position.x < temp.getWidth(); position.x++) {
			temp.setPixel(position, getPixel(position + a));
		}
	}
	return temp;
}

bool kl::Image::loadFromFile(const String& filePath) {
	uint64 gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (Warning(Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr), "Failed to init GDIPlus")) {
		return false;
	}

	{
		Gdiplus::Bitmap loadedBitmap(Strings::ToWString(filePath).c_str());
		Gdiplus::BitmapData bitmapData;
		if (Warning(loadedBitmap.GetLastStatus(), "Failed to open image file \"" + filePath + "\"")) {
			return false;
		}

		setSize(Int2(loadedBitmap.GetWidth(), loadedBitmap.GetHeight()));
		loadedBitmap.LockBits(nullptr, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
		memcpy(&m_Pixels[0], bitmapData.Scan0, m_Pixels.size() * sizeof(kl::Color));
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return true;
}

static constexpr CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
static constexpr CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
static constexpr CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

bool kl::Image::saveToFile(const String& fileName) const {
	const CLSID* formatToUse = nullptr;
	const String fileExtension = Files::GetExtension(fileName);

	if (fileExtension == ".bmp") {
		formatToUse = &bmpEncoderCLSID;
	}
	else if (fileExtension == ".jpg") {
		formatToUse = &jpgEncoderCLSID;
	}
	else if (fileExtension == ".png") {
		formatToUse = &pngEncoderCLSID;
	}
	else {
		if (fileExtension == ".txt") {
			std::ofstream file(fileName);
			if (file.is_open()) {
				for (uint y = 0; y < m_Size.y; y++) {
					for (uint x = 0; x < m_Size.x; x++) {
						Color pixel = m_Pixels[uint64(y) * m_Size.x + x];
						Write(file, x, " ", y, " => ", uint(pixel.r), " ", uint(pixel.g), " ", uint(pixel.b));
					}
				}
				file.close();
				return true;
			}
		}
		return false;
	}

	uint64 gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (Warning(Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr), "Failed to init GDIPlus")) {
		return false;
	}

	{
		Gdiplus::Bitmap tempBitmap(m_Size.x, m_Size.y, PixelFormat32bppARGB);
		Gdiplus::BitmapData bitmapData;

		tempBitmap.LockBits(nullptr, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
		memcpy(bitmapData.Scan0, &m_Pixels[0], m_Pixels.size() * sizeof(kl::Color));
		tempBitmap.UnlockBits(&bitmapData);

		tempBitmap.Save(Strings::ToWString(fileName).c_str(), formatToUse, nullptr);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return true;
}

void kl::Image::fill(const kl::Color& color) {
	for (auto& pixel : m_Pixels) {
		pixel = color;
	}
}

kl::Image kl::Image::flip(bool vertical) const {
	kl::Image temp(getSize());
	if (vertical) {
		for (uint x = 0; x < m_Size.x; x++) {
			for (uint y = 0; y < m_Size.y; y++) {
				temp.m_Pixels[uint64(y) * m_Size.x + x] = m_Pixels[(uint64(m_Size.y) - 1 - y) * m_Size.x + x];
			}
		}
	}
	else {
		for (uint y = 0; y < m_Size.y; y++) {
			for (uint x = 0; x < m_Size.x; x++) {
				temp.m_Pixels[uint64(y) * m_Size.x + x] = m_Pixels[uint64(y) * m_Size.x + m_Size.x - 1 - x];
			}
		}
	}
	return temp;
}

void kl::Image::drawLine(const kl::Int2& a, const kl::Int2& b, const kl::Color& color) {
	const int length = std::max(std::abs(b.x - a.x), std::abs(b.y - a.y));
	const Float2 incr(float(b.x - a.x) / length, float(b.y - a.y) / length);
	Float2 drawPoint(float(a.x), float(a.y));
	for (int i = 0; i <= length; i++) {
		setPixel(kl::Int2(int(drawPoint.x), int(drawPoint.y)), color);
		drawPoint += incr;
	}
}

void kl::Image::drawTriangle(Int2 a, Int2 b, Int2 c, const Color& color, bool fill) {
	if (fill) {
		if (a.y > b.y) {
			std::swap(a, b);
		}
		if (a.y > c.y) {
			std::swap(a, c);
		}
		if (b.y > c.y) {
			std::swap(b, c);
		}
		for (int y = a.y; y < c.y; y++) {
			drawLine(Int2(Math::GetLineX((y < b.y) ? a : c, b, float(y)), y), Int2(Math::GetLineX(a, c, float(y)), y), color);
		}
	}
	else {
		drawLine(a, b, color);
		drawLine(b, c, color);
		drawLine(c, a, color);
	}
}

void kl::Image::drawRectangle(Int2 a, Int2 b, const Color& color, bool fill) {
	if (fill) {
		if (a.y > b.y) {
			std::swap(a, b);
		}
		for (int y = a.y; y <= b.y; y++) {
			drawLine(kl::Int2(a.x, y), kl::Int2(b.x, y), color);
		}
	}
	else {
		drawLine(a, kl::Int2(a.x, b.y), color);
		drawLine(a, kl::Int2(b.x, a.y), color);
		drawLine(b, kl::Int2(a.x, b.y), color);
		drawLine(b, kl::Int2(b.x, a.y), color);
	}
}

void kl::Image::drawCircle(const Int2& p, float r, const Color& color, bool fill) {
	if (fill) {
		for (int y = int(p.y - r); y <= int(p.y + r); y++) {
			const int x = int(p.x + std::sqrt(r * r - (y - p.y) * (y - p.y)));
			drawLine(Int2(2 * p.x - x, y), Int2(x, y), color);
		}
	}
	else {
		for (int i = 0; i < 2 * r; i++) {
			const int x1 = int(p.x - r + i);
			const int y1 = int(p.y + std::sqrt(r * r - (x1 - p.x) * (x1 - p.x)));
			setPixel(kl::Int2(x1, y1), color);
			setPixel(kl::Int2(x1, 2 * p.y - y1), color);

			const int y2 = int(p.y - r + i);
			const int x2 = int(p.x + std::sqrt(r * r - (y2 - p.y) * (y2 - p.y)));
			setPixel(kl::Int2(x2, y2), color);
			setPixel(kl::Int2(2 * p.x - x2, y2), color);
		}
	}
}

void kl::Image::drawCircle(const kl::Int2& a, const kl::Int2& b, const kl::Color& color, bool fill) {
	drawCircle(a, Float2(b - a).length(), color, fill);
}

void kl::Image::drawImage(const Int2& position, const Image& image, bool mixAlpha) {
	if (mixAlpha) {
		for (UInt2 coords; coords.y < image.getHeight(); coords.y++) {
			for (coords.x = 0; coords.x < image.getWidth(); coords.x++) {
				setPixel(position + coords, getPixel(position + coords).mix(image.getPixel(coords)));
			}
		}
	}
	else {
		for (UInt2 coords; coords.y < image.getHeight(); coords.y++) {
			for (coords.x = 0; coords.x < image.getWidth(); coords.x++) {
				setPixel(position + coords, image.getPixel(coords));
			}
		}
	}
}

kl::String kl::Image::asASCII(const Int2& frameSize) const {
	StringStream frame;

	const kl::Int2 increment = m_Size / frameSize;
	for (kl::Int2 position; position.y < frameSize.y; position.y++) {
		for (position.x = 0; position.x < frameSize.x; position.x++) {
			frame << getPixel(position * increment).asASCII();
		}
	}

	return frame.str();
}
