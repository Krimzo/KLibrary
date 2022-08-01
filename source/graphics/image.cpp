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


kl::image::image() {
	size({});
}

kl::image::image(const kl::uint2& size, const kl::color& color) {
	this->size(size);
	fill(color);
}

kl::image::image(const std::string& filePath) {
	if (!fromFile(filePath)) {
		this->kl::image::image();
	}
}

std::vector<kl::color>::iterator kl::image::begin() {
	return m_Pixels.begin();
}

std::vector<kl::color>::iterator kl::image::end() {
	return m_Pixels.end();
}

uint kl::image::width() const {
	return m_Size.x;
}

void kl::image::width(uint width) {
	size(kl::int2(width, m_Size.y));
}

uint kl::image::height() const {
	return m_Size.y;
}

void kl::image::height(uint height) {
	size(kl::int2(m_Size.x, height));
}

kl::uint2 kl::image::size() const {
	return m_Size;
}

void kl::image::size(const kl::uint2& size) {
	if (size != m_Size) {
		m_Pixels.resize(uint64(size.x) * size.y);
		m_Size = size;
	}
}

kl::color kl::image::pixel(const kl::uint2& coords) const {
	if (coords.x < m_Size.x && coords.y < m_Size.y) {
		return m_Pixels[uint64(coords.y) * m_Size.x + coords.x];
	}
	return kl::colors::black;
}

void kl::image::pixel(const kl::uint2& coords, const kl::color& color) {
	if (coords.x < m_Size.x && coords.y < m_Size.y) {
		m_Pixels[uint64(coords.y) * m_Size.x + coords.x] = color;
	}
}

kl::color* kl::image::data() {
	return &m_Pixels[0];
}

const kl::color* kl::image::data() const {
	return &m_Pixels[0];
}

kl::image kl::image::rect(kl::uint2 a, kl::uint2 b) const {
	if (b.x < a.x) {
		std::swap(a.x, b.x);
	}
	if (b.y < a.y) {
		std::swap(a.y, b.y);
	}
	kl::image temp(b - a);
	for (kl::uint2 pos; pos.y < temp.height(); pos.y++) {
		for (pos.x = 0; pos.x < temp.width(); pos.x++) {
			temp.pixel(pos, pixel(pos + a));
		}
	}
	return temp;
}

bool kl::image::fromFile(const std::string& filePath) {
	uint64 gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (kl::console::warning(Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr), "Failed to init GDIPlus")) {
		return false;
	}

	{
		Gdiplus::Bitmap loadedBitmap(kl::to::wstring(filePath).c_str());
		Gdiplus::BitmapData bitmapData;
		if (kl::console::warning(loadedBitmap.GetLastStatus(), "Failed to open image file \"" + filePath + "\"")) {
			return false;
		}
		size(kl::int2(loadedBitmap.GetWidth(), loadedBitmap.GetHeight()));
		loadedBitmap.LockBits(nullptr, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
		memcpy(&m_Pixels[0], bitmapData.Scan0, m_Pixels.size() * sizeof(kl::color));
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return true;
}

static constexpr CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
static constexpr CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
static constexpr CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

bool kl::image::toFile(const std::string& fileName) const {
	const CLSID* formatToUse = nullptr;
	const std::string fileExtension = kl::file::extension(fileName);
	if (fileExtension == ".bmp") {
		formatToUse = &bmpEncoderCLSID;
	}
	else if (fileExtension == ".jpg") {
		formatToUse = &jpgEncoderCLSID;
	}
	else if (fileExtension == ".png") {
		formatToUse = &pngEncoderCLSID;
	}
	else if (fileExtension == ".txt") {
		std::stringstream ss;
		for (uint y = 0; y < m_Size.y; y++)
			for (uint x = 0; x < m_Size.x; x++)
				ss <<
				x << " " << y << " => " <<
				uint(m_Pixels[uint64(y) * m_Size.x + x].r) << " " <<
				uint(m_Pixels[uint64(y) * m_Size.x + x].g) << " " <<
				uint(m_Pixels[uint64(y) * m_Size.x + x].b) << "\n";
		kl::file::writeString(fileName, ss.str());
		return true;
	}
	else {
		return false;
	}

	uint64 gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (kl::console::warning(Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr), "Failed to init GDIPlus")) {
		return false;
	}

	{
		Gdiplus::Bitmap tempBitmap(m_Size.x, m_Size.y, PixelFormat32bppARGB);
		Gdiplus::BitmapData bitmapData;
		tempBitmap.LockBits(nullptr, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
		memcpy(bitmapData.Scan0, &m_Pixels[0], m_Pixels.size() * sizeof(kl::color));
		tempBitmap.UnlockBits(&bitmapData);
		tempBitmap.Save(kl::to::wstring(fileName).c_str(), formatToUse, nullptr);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return true;
}

void kl::image::fill(const kl::color& color) {
	for (auto& pixel : m_Pixels) {
		pixel = color;
	}
}

kl::image kl::image::flipH() const {
	kl::image temp(size());
	for (uint y = 0; y < m_Size.y; y++) {
		for (uint x = 0; x < m_Size.x; x++) {
			temp.m_Pixels[uint64(y) * m_Size.x + x] = m_Pixels[uint64(y) * m_Size.x + m_Size.x - 1 - x];
		}
	}
	return temp;
}

kl::image kl::image::flipV() const {
	kl::image temp(size());
	for (uint x = 0; x < m_Size.x; x++) {
		for (uint y = 0; y < m_Size.y; y++) {
			temp.m_Pixels[uint64(y) * m_Size.x + x] = m_Pixels[(uint64(m_Size.y) - 1 - y) * m_Size.x + x];
		}
	}
	return temp;
}

void kl::image::drawLine(const kl::int2& a, const kl::int2& b, const kl::color& col) {
	const int length = std::max(std::abs(b.x - a.x), std::abs(b.y - a.y));
	const kl::float2 incr(float(b.x - a.x) / length, float(b.y - a.y) / length);
	kl::float2 drawPoint(float(a.x), float(a.y));
	for (int i = 0; i <= length; i++) {
		pixel(kl::int2(int(drawPoint.x), int(drawPoint.y)), col);
		drawPoint += incr;
	}
}

void kl::image::drawTriangle(kl::int2 a, kl::int2 b, kl::int2 c, const kl::color& col, bool fill) {
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
			drawLine(kl::int2(int(kl::math::lineX<float>((y < b.y) ? a : c, b, float(y))), y),
				kl::int2(int(kl::math::lineX<float>(a, c, float(y))), y), col);
		}
	}
	else {
		drawLine(a, b, col);
		drawLine(b, c, col);
		drawLine(c, a, col);
	}
}

void kl::image::drawRectangle(kl::int2 a, kl::int2 b, const kl::color& col, bool fill) {
	if (fill) {
		if (a.y > b.y) {
			std::swap(a, b);
		}
		for (int y = a.y; y <= b.y; y++) {
			drawLine(kl::int2(a.x, y), kl::int2(b.x, y), col);
		}
	}
	else {
		drawLine(a, kl::int2(a.x, b.y), col);
		drawLine(a, kl::int2(b.x, a.y), col);
		drawLine(b, kl::int2(a.x, b.y), col);
		drawLine(b, kl::int2(b.x, a.y), col);
	}
}

void kl::image::drawCircle(const kl::int2& p, float r, const kl::color& col, bool fill) {
	if (fill) {
		for (int y = int(p.y - r); y <= int(p.y + r); y++) {
			const int x = int(p.x + std::sqrt(r * r - (y - p.y) * (y - p.y)));
			drawLine(kl::int2(2 * p.x - x, y), kl::int2(x, y), col);
		}
	}
	else {
		for (int i = 0; i < 2 * r; i++) {
			const int x1 = int(p.x - r + i);
			const int y1 = int(p.y + std::sqrt(r * r - (x1 - p.x) * (x1 - p.x)));
			pixel(kl::int2(x1, y1), col);
			pixel(kl::int2(x1, 2 * p.y - y1), col);

			const int y2 = int(p.y - r + i);
			const int x2 = int(p.x + std::sqrt(r * r - (y2 - p.y) * (y2 - p.y)));
			pixel(kl::int2(x2, y2), col);
			pixel(kl::int2(2 * p.x - x2, y2), col);
		}
	}
}
void kl::image::drawCircle(const kl::int2& a, const kl::int2& b, const kl::color& col, bool fill) {
	drawCircle(a, kl::float2(b - a).length(), col, fill);
}

void kl::image::drawImage(const kl::int2& pos, const kl::image& img, bool mixAlpha) {
	for (kl::uint2 coords; coords.y < img.height(); coords.y++) {
		for (coords.x = 0; coords.x < img.width(); coords.x++) {
			if (mixAlpha) {
				pixel(pos + coords, pixel(pos + coords).mix(img.pixel(coords)));
			}
			else {
				pixel(pos + coords, img.pixel(coords));
			}
		}
	}
}

std::string kl::image::ascii(const kl::int2& frameSize) const {
	const kl::int2 incr = m_Size / frameSize;
	std::stringstream frame;
	for (kl::int2 pos; pos.y < frameSize.y; pos.y++) {
		for (pos.x = 0; pos.x < frameSize.x; pos.x++) {
			frame << pixel(pos * incr).ascii();
		}
	}
	return frame.str();
}
