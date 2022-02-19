#include "KrimzLib/graphics/image.h"

#include <iostream>
#include <sstream>
#include <windows.h>
#include <gdiplus.h>

#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/file.h"
#include "KrimzLib/math.h"


// Constructor
kl::image::image() {
	this->setSize(kl::ivec2());
}
kl::image::image(const kl::ivec2& size, const kl::color& color) {
	this->setSize(size);
	this->fill(color);
}
kl::image::image(const char* fileName) {
	this->fromFile(fileName);
}

// Getters
int kl::image::getWidth() const {
	return width;
}
int kl::image::getHeight() const {
	return height;
}
kl::ivec2 kl::image::getSize() const {
	return kl::ivec2(width, height);
}
kl::color kl::image::getPixel(const kl::ivec2& point) const {
	if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
		return pixels[size_t(point.y) * width + point.x];
	}
	return kl::colors::purple;
}
int kl::image::pixelCount() const {
	return int(pixels.size());
}
byte* kl::image::pointer() const {
	return (byte*)&pixels[0];
}
kl::image kl::image::getRect(kl::ivec2 a, kl::ivec2 b) const {
	// Clamping
	a.x = min(max(a.x, 0), this->width);
	a.y = min(max(a.y, 0), this->height);
	b.x = min(max(b.x, 0), this->width);
	b.y = min(max(b.y, 0), this->height);

	// Sorting
	if (b.x < a.x) {
		int t = a.x;
		a.x = b.x;
		b.x = t;
	}
	if (b.y < a.y) {
		int t = a.y;
		a.y = b.y;
		b.y = t;
	}

	// Saving data
	kl::image temp(b - a);
	for (int y = 0; y < temp.getHeight(); y++) {
		for (int x = 0; x < temp.getWidth(); x++) {
			const kl::ivec2 pos(x, y);
			temp.setPixel(pos, this->getPixel(pos + a));
		}
	}
	return temp;
}

// Setters
void kl::image::setWidth(int width) {
	setSize(kl::ivec2(width, height));
}
void kl::image::setHeight(int height) {
	setSize(kl::ivec2(width, height));
}
void kl::image::setSize(const kl::ivec2& size) {
	width = size.x;
	height = size.y;
	pixels.resize(size_t(width) * height);
}
void kl::image::setPixel(const kl::ivec2& point, const kl::color& color) {
	if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
		pixels[size_t(point.y) * width + point.x] = color;
	}
}

// Reads an image file and stores it in the image instance
void kl::image::fromFile(const std::string& filePath) {
	// Gdiplus init
	ULONG_PTR gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr)) {
		std::cout << "GdiPlus: Failed to init!";
		std::cin.get();
		exit(69);
	}

	// Loads image file
	Gdiplus::Bitmap* loadedBitmap = new Gdiplus::Bitmap(kl::convert::toWString(filePath).c_str());

	// Checks load status
	if (loadedBitmap->GetLastStatus()) {
		printf("Could not load image file \"%s\".", filePath.c_str());
		std::cin.get();
		exit(69);
	}

	// Resizing the self
	this->setSize(kl::ivec2(loadedBitmap->GetWidth(), loadedBitmap->GetHeight()));

	// Locking the bitmap data
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect bitmapRect(0, 0, width, height);
	loadedBitmap->LockBits(&bitmapRect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);

	// Loading the pixel data
	const int bitmapStride = bitmapData.Stride / 4;
	const uint32_t* bmPixels = (uint32_t*)bitmapData.Scan0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			pixels[size_t(y) * width + x] = *(kl::color*)(bmPixels + y * bitmapStride + x);
		}
	}

	// Cleanup
	delete loadedBitmap;
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

// Saves the image to a file
void kl::image::toFile(const std::string& fileName) const {
	// Static image type CLSID-s
	static const CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
	static const CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
	static const CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };

	// Checking the file extension is supported
	const CLSID* formatToUse = nullptr;
	std::string fileExtension = kl::file::getExtension(fileName);
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
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				ss <<
					x << " " << y << " => " <<
					int(pixels[(uint64_t)y * width + x].r) << " " <<
					int(pixels[(uint64_t)y * width + x].g) << " " <<
					int(pixels[(uint64_t)y * width + x].b) << "\n";
			}
		}
		kl::file::write(fileName, ss.str());
		return;
	}
	else {
		printf("File extension \"%s\" is not supported!\n", fileExtension.c_str());
		return;
	}

	// Gdiplus init
	ULONG_PTR gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr)) {
		std::cout << "GdiPlus: Failed to init!";
		std::cin.get();
		exit(69);
	}

	// Pixel data transfer and saving to file
	Gdiplus::Bitmap* tempBitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			kl::color tempPixel = getPixel(kl::ivec2(x, y));
			tempBitmap->SetPixel(int(x), int(y), Gdiplus::Color(tempPixel.a, tempPixel.r, tempPixel.g, tempPixel.b));
		}
	}
	tempBitmap->Save(kl::convert::toWString(fileName).c_str(), formatToUse, nullptr);

	// Bitmap deletion
	delete tempBitmap;

	// Gdiplus cleanup
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

// Fils the image with solid color
void kl::image::fill(const kl::color& color) {
	for (int i = 0; i < pixels.size(); i++) {
		pixels[i] = color;
	}
}

// Flips the pixel on x axis
kl::image kl::image::flipHorizontal() const {
	// Temp image
	kl::image temp(this->getSize());

	// Flipping
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			temp.pixels[size_t(y) * width + x] = this->pixels[size_t(y) * width + width - 1 - x];
		}
	}

	// Returning the edited
	return temp;
}

// Flips the pixel on y axis
kl::image kl::image::flipVertical() const {
	// Temp image
	kl::image temp(this->getSize());

	// Flipping
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			temp.pixels[size_t(y) * width + x] = this->pixels[size_t(height - 1 - y) * width + x];
		}
	}

	// Returning the edited
	return temp;
}

// Draws a line between 2 points
void kl::image::drawLine(const kl::ivec2& a, const kl::ivec2& b, const kl::color& col) {
	// Calculations
	const int len = max(abs(b.x - a.x), abs(b.y - a.y));
	kl::vec2 incr((float(b.x) - a.x) / len, (float(b.y) - a.y) / len);

	// Drawing
	kl::vec2 drawPoint(float(a.x), float(a.y));
	for (int i = 0; i <= len; i++) {
		setPixel(kl::ivec2(int(drawPoint.x), int(drawPoint.y)), col);
		drawPoint = drawPoint + incr;
	}
}

// Draws a triangle between 3 points
void kl::image::drawTriangle(kl::ivec2 a, kl::ivec2 b, kl::ivec2 c, const kl::color& col, bool fill) {
	if (fill) {
		// Sorting by y
		if (a.y > b.y) {
			kl::ivec2 temp = a;
			a = b;
			b = temp;
		}
		if (a.y > c.y) {
			kl::ivec2 temp = a;
			a = c;
			c = temp;
		}
		if (b.y > c.y) {
			kl::ivec2 temp = b;
			b = c;
			c = temp;
		}

		// Drawing
		for (int y = a.y; y < c.y; y++) {
			drawLine(kl::ivec2(int(kl::math::lineX((y < b.y) ? a : c, b, float(y))), int(y)), kl::ivec2(int(kl::math::lineX(a, c, float(y))), int(y)), col);
		}
	}
	else {
		drawLine(a, b, col);
		drawLine(b, c, col);
		drawLine(c, a, col);
	}
}

// Draws a rectangle between 2 points
void kl::image::drawRectangle(kl::ivec2 a, kl::ivec2 b, const kl::color& col, bool fill) {
	if (fill) {
		// Sorting by y
		if (a.y > b.y) {
			kl::ivec2 temp = a;
			a = b;
			b = temp;
		}

		// Drawing
		for (int y = a.y; y <= b.y; y++) {
			drawLine(kl::ivec2(a.x, y), kl::ivec2(b.x, y), col);
		}
	}
	else {
		drawLine(a, kl::ivec2(a.x, b.y), col);
		drawLine(a, kl::ivec2(b.x, a.y), col);
		drawLine(b, kl::ivec2(a.x, b.y), col);
		drawLine(b, kl::ivec2(b.x, a.y), col);
	}
}

// Draws a circle with the given center point and radius
void kl::image::drawCircle(const kl::ivec2& p, float r, const kl::color& col, bool fill) {
	if (fill) {
		for (int y = int(p.y - r); y <= int(p.y + r); y++) {
			const int x = int(p.x + sqrt(r * r - float(y - p.y) * float(y - p.y)));
			drawLine(kl::ivec2(2 * p.x - x, y), kl::ivec2(x, y), col);
		}
	}
	else {
		for (int i = 0; i < 2 * r; i++) {
			// X run
			const int x1 = int(p.x - r + i);
			const int y1 = int(p.y + sqrt(r * r - float(x1 - p.x) * float(x1 - p.x)));
			setPixel(kl::ivec2(x1, y1), col);
			setPixel(kl::ivec2(x1, 2 * p.y - y1), col);

			// Y run
			const int y2 = int(p.y - r + i);
			const int x2 = int(p.x + sqrt(r * r - float(y2 - p.y) * float(y2 - p.y)));
			setPixel(kl::ivec2(x2, y2), col);
			setPixel(kl::ivec2(2 * p.x - x2, y2), col);
		}
	}
}
// Draws a circle between 1 center and 1 outer point
void kl::image::drawCircle(const kl::ivec2& a, const kl::ivec2& b, const kl::color& col, bool fill) {
	this->drawCircle(a, kl::vec2(a, b).length(), col, fill);
}

// Converts an image to an ASCII frame
std::string kl::image::toASCII(const kl::ivec2& frameSize) {
	// ASCII 'table'
	static const char asciiPixelTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };

	// Calculations
	const int pixelWidthIncrement = width / frameSize.x;
	const int pixelHeightIncrement = height / frameSize.y;

	// Processing
	std::stringstream frame;
	for (int y = 0; y < frameSize.y; y++) {
		for (int x = 0; x < frameSize.x; x++) {
			// Pixels to grayscale
			kl::color currentPixel = getPixel(kl::ivec2(x * pixelWidthIncrement, y * pixelHeightIncrement));
			const int grayScaledPixel = int(currentPixel.r * 0.299f + currentPixel.g * 0.587f + currentPixel.b * 0.114f);

			// Grayscaled values to ASCII
			static const float toSatur = 9.0f / 255;
			const int saturationLevel = int(grayScaledPixel * toSatur);
			frame << asciiPixelTable[saturationLevel];
		}
		frame << '\n';
	}
	return frame.str();
}
