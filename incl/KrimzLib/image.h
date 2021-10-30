#pragma once


namespace kl {
	class image {
	public:
		// Constructor
		image(size size, color color = {}) {
			SetSize(size);
			FillSolid(color);
		}
		image(const char* fileName) {
			FromFile(fileName);
		}

		// Getters
		int GetWidth() {
			return width;
		}
		int GetHeight() {
			return height;
		}
		size GetSize() {
			return { width, height };
		}
		color GetPixel(point point) {
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
				return pixels[point.y * size_t(width) + point.x];
			}
			return { 0, 0, 0 };
		}
		size_t GetPixelCount() {
			return pixels.size();
		}
		color* GetRawData() {
			return &pixels[0];
		}

		// Setters
		void SetWidth(int width) {
			SetSize({ width, height });
		}
		void SetHeight(int height) {
			SetSize({ width, height });
		}
		void SetSize(size size) {
			width = size.width;
			height = size.height;
			pixels.resize(size_t(width) * size_t(height));
		}
		void SetPixel(point point, color color) {
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
				pixels[point.y * size_t(width) + point.x] = color;
			}
		}
		
		// Reads an image file and stores it in the image instance
		void FromFile(std::string filePath) {
			
		}

		// Saves the image to a file
		void ToFile(std::string fileName) {
			
		}

		// Fils the image with solid color
		void FillSolid(color color) {
			std::fill(pixels.begin(), pixels.end(), color);
		}
		
		// Resets the byte values
		void FastClear(byte value = 0) {
			memset(&pixels[0], value, pixels.size() * sizeof(color));
		}

		// Draws a line between 2 points
		void DrawLine(point a, point b, color c) {
			// Calculations
			int len = std::max(abs(b.x - a.x), abs(b.y - a.y));
			vec2 incr = { double(b.x - a.x) / len, double(b.y - a.y) / len };

			// Drawing
			vec2 drawPoint = { (double)a.x, (double)a.y };
			for (int i = 0; i < len; i++) {
				SetPixel({ (int)drawPoint.x, (int)drawPoint.y }, c);
				drawPoint = drawPoint + incr;
			}
		}

		// Draws a rectangle between 2 points
		void DrawRect(point a, point b, color c, bool fill = false) {
			if (fill) {
				point topLeft = { std::min(a.x, b.x), std::min(a.y, b.y) };
				point bottomRight = { std::max(a.x, b.x), std::max(a.y, b.y) };
				for (int y = topLeft.y; y < bottomRight.y; y++) {
					for (int x = topLeft.x; x < bottomRight.x; x++) {
						SetPixel({ x, y }, c);
					}
				}
			}
			else {
				DrawLine(a, { a.x, b.y }, c);
				DrawLine(a, { b.x, a.y }, c);
				DrawLine(b, { a.x, b.y }, c);
				DrawLine(b, { b.x, a.y }, c);
			}
		}

		// Converts an image to an ASCII frame
		std::string ToASCII(size frameSize) {
			static const char asciiPixelTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };

			// Calculations
			int pixelWidthIncrement = width / frameSize.width;
			int pixelHeightIncrement = height / frameSize.height;

			// Processing
			std::stringstream frame;
			for (int y = 0; y < frameSize.height; y++) {
				for (int x = 0; x < frameSize.width; x++) {
					// Pixels to grayscale
					color currentPixel = GetPixel({ x * pixelWidthIncrement, y * pixelHeightIncrement });
					int grayScaledPixel = (int)(currentPixel.r * 0.299 + currentPixel.g * 0.587 + currentPixel.b * 0.114);

					// Grayscaled values to ASCII
					int saturationLevel = (int)((grayScaledPixel / 255.0) * 9);
					frame << asciiPixelTable[saturationLevel];
				}
				frame << '\n';
			}
			return frame.str();
		}

	private:
		int width = 0;
		int height = 0;
		std::vector<color> pixels = {};
	};
}