#include "graphics/image.h"

#include <iostream>
#include <sstream>
#include <windows.h>
#include <gdiplus.h>

#include "math/math.h"
#include "math/float2.h"
#include "utility/convert.h"
#include "utility/file.h"


// Constructor
kl::image::image() {
	resize(0);
}
kl::image::image(const kl::int2& size, const kl::color& color) {
	resize(size);
	fill(color);
}
kl::image::image(const String& filePath) {
	if (!fromFile(filePath)) {
		this->kl::image::image();
	}
}

// Iterator
std::vector<kl::color>::iterator kl::image::begin() {
	return pixels.begin();
}
std::vector<kl::color>::iterator kl::image::end() {
	return pixels.end();
}

// Getters
int kl::image::width() const {
	return _size.x;
}
int kl::image::height() const {
	return _size.y;
}
kl::int2 kl::image::size() const {
	return _size;
}
kl::color kl::image::gpixel(const kl::int2& coords) const {
	if (coords.x >= 0 && coords.x < _size.x && coords.y >= 0 && coords.y < _size.y) {
		return pixels[size_t(coords.y * _size.x + coords.x)];
	}
	return kl::colors::black;
}
kl::color* kl::image::pointer() const {
	return (kl::color*)&pixels.front();
}
kl::image kl::image::rect(kl::int2 a, kl::int2 b) const {
	// Sorting
	if (b.x < a.x) {
		std::swap(a.x, b.x);
	}
	if (b.y < a.y) {
		std::swap(a.y, b.y);
	}

	// Saving data
	kl::image temp(b - a);
	for (kl::int2 pos = 0; pos.y < temp.height(); pos.y++) {
		for (pos.x = 0; pos.x < temp.width(); pos.x++) {
			temp.spixel(pos, gpixel(pos + a));
		}
	}
	return temp;
}

// Setters
void kl::image::width(int width) {
	resize(kl::int2(width, _size.y));
}
void kl::image::height(int height) {
	resize(kl::int2(_size.x, height));
}
void kl::image::resize(const kl::int2& size) {
	if (size != _size) {
		pixels.resize(size_t(size.x * size.y));
		_size = size;
	}
}
void kl::image::spixel(const kl::int2& coords, const kl::color& color) {
	if (coords.x >= 0 && coords.x < _size.x && coords.y >= 0 && coords.y < _size.y) {
		pixels[size_t(coords.y * _size.x + coords.x)] = color;
	}
}

// Reads an image file and stores it in the image instance
bool kl::image::fromFile(const String& filePath) {
	// Gdiplus init
	uint64_t gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr)) {
		std::cout << "Image: GdiPlus failed to init!" << std::endl;
		return false;
	}

	// Loading image file
	Gdiplus::Bitmap* loadedBitmap = new Gdiplus::Bitmap(kl::convert::toWString(filePath).c_str());
	if (loadedBitmap->GetLastStatus()) {
		std::cout << "Image: Could not open file \"" << filePath << "\"!" << std::endl;
		return false;
	}

	// Resizing the self
	resize(kl::int2(loadedBitmap->GetWidth(), loadedBitmap->GetHeight()));

	// Locking the bitmap data
	Gdiplus::BitmapData bitmapData;
	loadedBitmap->LockBits(nullptr, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);

	// Loading the pixel data
	memcpy(&pixels[0], bitmapData.Scan0, pixels.size() * sizeof(kl::color));

	// Cleanup
	delete loadedBitmap;
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return true;
}

// Saves the image to a file
const CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
const CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
const CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
bool kl::image::toFile(const String& fileName) const {
	// Checking the file extension is supported
	const CLSID* formatToUse = nullptr;
	String fileExtension = kl::file::getExtension(fileName);
	if (fileExtension == "bmp") {
		formatToUse = &bmpEncoderCLSID;
	}
	else if (fileExtension == "jpg") {
		formatToUse = &jpgEncoderCLSID;
	}
	else if (fileExtension == "png") {
		formatToUse = &pngEncoderCLSID;
	}
	else if (fileExtension == "txt") {
		std::stringstream ss;
		for (int y = 0; y < _size.y; y++)
			for (int x = 0; x < _size.x; x++)
				ss <<
				x << " " << y << " => " <<
				int(pixels[size_t(y) * _size.x + x].r) << " " <<
				int(pixels[size_t(y) * _size.x + x].g) << " " <<
				int(pixels[size_t(y) * _size.x + x].b) << "\n";
		kl::file::write(fileName, ss.str());
		return true;
	}
	else {
		std::cout << "Image: File extension \"" << fileExtension << "\" is not supported!" << std::endl;
		return false;
	}

	// Gdiplus init
	ULONG_PTR gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr)) {
		std::cout << "Image: GdiPlus failed to init!" << std::endl;
		return false;
	}

	// Temp bitmap creation
	Gdiplus::Bitmap* tempBitmap = new Gdiplus::Bitmap(_size.x, _size.y, PixelFormat32bppARGB);

	// Locking the bitmap data
	Gdiplus::BitmapData bitmapData;
	tempBitmap->LockBits(nullptr, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

	// Pixel data transfer
	memcpy(bitmapData.Scan0, &pixels[0], pixels.size() * sizeof(kl::color));

	// Unlocking the bitmap
	tempBitmap->UnlockBits(&bitmapData);

	// Saving to file
	tempBitmap->Save(kl::convert::toWString(fileName).c_str(), formatToUse, nullptr);

	// Cleanup
	delete tempBitmap;
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return true;
}

// Fils the image with solid color
void kl::image::fill(const kl::color& color) {
	for (int i = 0; i < pixels.size(); i++) {
		pixels[i] = color;
	}
}

// Flips the pixel on x axis
kl::image kl::image::flipH() const {
	// Temp image
	kl::image temp(size());

	// Flipping
	for (int y = 0; y < _size.y; y++) {
		for (int x = 0; x < _size.x; x++) {
			temp.pixels[size_t(y) * _size.x + x] = pixels[size_t(y) * _size.x + _size.x - 1 - x];
		}
	}

	// Returning the edited
	return temp;
}

// Flips the pixel on y axis
kl::image kl::image::flipV() const {
	// Temp image
	kl::image temp(size());

	// Flipping
	for (int x = 0; x < _size.x; x++) {
		for (int y = 0; y < _size.y; y++) {
			temp.pixels[size_t(y) * _size.x + x] = pixels[size_t(_size.y - 1 - y) * _size.x + x];
		}
	}

	// Returning the edited
	return temp;
}

// Draws a line between 2 points
void kl::image::drawLine(const kl::int2& a, const kl::int2& b, const kl::color& col) {
	// Calculations
	const int len = max(abs(b.x - a.x), abs(b.y - a.y));
	const kl::float2 incr(float(b.x - a.x) / len, float(b.y - a.y) / len);

	// Drawing
	kl::float2 drawPoint(float(a.x), float(a.y));
	for (int i = 0; i <= len; i++) {
		spixel(kl::int2(int(drawPoint.x), int(drawPoint.y)), col);
		drawPoint += incr;
	}
}

// Draws a triangle between 3 points
void kl::image::drawTriangle(kl::int2 a, kl::int2 b, kl::int2 c, const kl::color& col, bool fill) {
	if (fill) {
		// Sorting by y
		if (a.y > b.y) {
			std::swap(a, b);
		}
		if (a.y > c.y) {
			std::swap(a, c);
		}
		if (b.y > c.y) {
			std::swap(b, c);
		}

		// Drawing
		for (int y = a.y; y < c.y; y++) {
			drawLine(kl::int2(int(kl::math::lineX((y < b.y) ? a : c, b, float(y))), int(y)), kl::int2(int(kl::math::lineX(a, c, float(y))), int(y)), col);
		}
	}
	else {
		drawLine(a, b, col);
		drawLine(b, c, col);
		drawLine(c, a, col);
	}
}

// Draws a rectangle between 2 points
void kl::image::drawRectangle(kl::int2 a, kl::int2 b, const kl::color& col, bool fill) {
	if (fill) {
		// Sorting by y
		if (a.y > b.y) {
			std::swap(a, b);
		}

		// Drawing
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

// Draws a circle with the given center point and radius
void kl::image::drawCircle(const kl::int2& p, float r, const kl::color& col, bool fill) {
	if (fill) {
		for (int y = int(p.y - r); y <= int(p.y + r); y++) {
			const int x = int(p.x + sqrt(r * r - float(y - p.y) * float(y - p.y)));
			drawLine(kl::int2(2 * p.x - x, y), kl::int2(x, y), col);
		}
	}
	else {
		for (int i = 0; i < 2 * r; i++) {
			// X run
			const int x1 = int(p.x - r + i);
			const int y1 = int(p.y + sqrt(r * r - float(x1 - p.x) * float(x1 - p.x)));
			spixel(kl::int2(x1, y1), col);
			spixel(kl::int2(x1, 2 * p.y - y1), col);

			// Y run
			const int y2 = int(p.y - r + i);
			const int x2 = int(p.x + sqrt(r * r - float(y2 - p.y) * float(y2 - p.y)));
			spixel(kl::int2(x2, y2), col);
			spixel(kl::int2(2 * p.x - x2, y2), col);
		}
	}
}
// Draws a circle between 1 center and 1 outer point
void kl::image::drawCircle(const kl::int2& a, const kl::int2& b, const kl::color& col, bool fill) {
	drawCircle(a, kl::float2(b - a).length(), col, fill);
}

// Draws image on image
void kl::image::drawImage(const kl::int2& pos, const kl::image& img, bool mixAlpha) {
	for (kl::int2 coords = 0; coords.y < img.height(); coords.y++) {
		for (coords.x = 0; coords.x < img.width(); coords.x++) {
			const kl::color imgPxl = img.gpixel(coords);
			if (mixAlpha) {
				spixel(pos + coords, gpixel(pos + coords).mix(img.gpixel(coords)));
			}
			else {
				spixel(pos + coords, img.gpixel(coords));
			}
		}
	}
}

// Converts an image to an ASCII frame
String kl::image::toASCII(const kl::int2& frameSize) const {
	const kl::int2 incr = _size / frameSize;
	std::stringstream frame;
	for (kl::int2 pos = 0; pos.y < frameSize.y; pos.y++) {
		for (pos.x = 0; pos.x < frameSize.x; pos.x++) {
			frame << gpixel(pos * incr).toASCII();
		}
	}
	return frame.str();
}
