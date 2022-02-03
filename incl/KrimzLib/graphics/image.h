#pragma once


namespace kl {
	namespace _kl_dont_use_ {
		ULONG_PTR gdiplusToken = NULL;
		Gdiplus::GdiplusStartupInput gdiplusStartupInput = {};
		struct _kl_image_initialization_ {
			_kl_image_initialization_() {
				kl::console::error(Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr), "GdiPlus: Failed to init!");
			}
			~_kl_image_initialization_() {
				Gdiplus::GdiplusShutdown(gdiplusToken);
			}
		};
		_kl_image_initialization_ _image_init_;
	}

	class image {
	public:
		// Constructor
		image() {
			setSize(kl::ivec2(0, 0));
		}
		image(kl::ivec2 size, kl::color color = {}) {
			setSize(size);
			fillSolid(color);
		}
		image(const char* fileName) {
			fromFile(fileName);
		}

		// Getters
		int getWidth() {
			return width;
		}
		int getHeight() {
			return height;
		}
		kl::ivec2 getSize() {
			return kl::ivec2(width, height);
		}
		kl::color getPixel(kl::ivec2 point) {
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
				return pixels[(uint64_t)point.y * width + point.x];
			}
			return kl::color();
		}
		int getPixelCount() {
			return (int)pixels.size();
		}
		kl::byte* pointer() {
			return (kl::byte*)&pixels[0];
		}

		// Setters
		void setWidth(int width) {
			setSize(kl::ivec2(width, height));
		}
		void setHeight(int height) {
			setSize(kl::ivec2(width, height));
		}
		void setSize(kl::ivec2 size) {
			width = size.x;
			height = size.y;
			pixels.resize((uint64_t)width * height);
		}
		void setPixel(kl::ivec2 point, kl::color color) {
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
				pixels[(uint64_t)point.y * width + point.x] = color;
			}
		}

		// Reads an image file and stores it in the image instance
		void fromFile(String filePath) {
			// Loads image file
			Gdiplus::Bitmap loadedBitmap(kl::convert::toWString(filePath).c_str());

			// Checks load status
			if (loadedBitmap.GetLastStatus()) {
				printf("Could not load image file \"%s\".", filePath.c_str());
				exit(69);
			}

			// Resizing the self
			this->setSize(kl::ivec2(loadedBitmap.GetWidth(), loadedBitmap.GetHeight()));

			// Locking the bitmap data
			Gdiplus::BitmapData bitmapData;
			Gdiplus::Rect bitmapRect(0, 0, width, height);
			loadedBitmap.LockBits(&bitmapRect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);

			// Loading the pixel data
			const int bitmapStride = bitmapData.Stride / 4;
			const uint32_t* bmPixels = (uint32_t*)bitmapData.Scan0;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pixels[(uint64_t)y * width + x] = *(kl::color*)(bmPixels + y * bitmapStride + x);
				}
			}
		}

		// Saves the image to a file
		void toFile(String fileName) {
			static const CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };

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

			// Pixel data transfer and saving to file
			Gdiplus::Bitmap tempBitmap(width, height, PixelFormat24bppRGB);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					kl::color tempPixel = getPixel(kl::ivec2(x, y));
					tempBitmap.SetPixel(int(x), int(y), Gdiplus::Color(tempPixel.r, tempPixel.g, tempPixel.b));
				}
			}
			tempBitmap.Save(kl::convert::toWString(fileName).c_str(), formatToUse, nullptr);
		}

		// Fils the image with solid color
		void fillSolid(kl::color color) {
			for (int i = 0; i < pixels.size(); i++) {
				pixels[i] = color;
			}
		}

		// Flips the pixel on x axis
		void flipHorizontal() {
			const int halfWidth = width / 2;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < halfWidth; x++) {
					kl::color tempPixel = pixels[(uint64_t)y * width + x];
					pixels[(uint64_t)y * width + x] = pixels[(uint64_t)y * width + uint64_t(width - 1 - x)];
					pixels[(uint64_t)y * width + uint64_t(width - 1 - x)] = tempPixel;
				}
			}
		}

		// Flips the pixel on y axis
		void flipVertical() {
			const int halfHeight = height / 2;
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < halfHeight; y++) {
					kl::color tempPixel = pixels[(uint64_t)y * width + x];
					pixels[(uint64_t)y * width + x] = pixels[uint64_t(height - 1 - y) * width + x];
					pixels[uint64_t(height - 1 - y) * width + x] = tempPixel;
				}
			}
		}

		// Draws a line between 2 points
		void drawLine(kl::ivec2 a, kl::ivec2 b, kl::color col) {
			// Calculations
			int len = std::max(abs(b.x - a.x), abs(b.y - a.y));
			kl::vec2 incr(((float)b.x - a.x) / len, ((float)b.y - a.y) / len);

			// Drawing
			kl::vec2 drawPoint((float)a.x, (float)a.y);
			for (int i = 0; i <= len; i++) {
				setPixel(kl::ivec2((int)drawPoint.x, (int)drawPoint.y), col);
				drawPoint = drawPoint + incr;
			}
		}

		// Draws a triangle between 3 points
		void drawTriangle(kl::ivec2 a, kl::ivec2 b, kl::ivec2 c, kl::color col, bool fill = false) {
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
					drawLine(kl::ivec2((int)kl::math::lineX((y < b.y) ? a : c, b, (float)y), (int)y), kl::ivec2((int)kl::math::lineX(a, c, (float)y), (int)y), col);
				}
			}
			else {
				drawLine(a, b, col);
				drawLine(b, c, col);
				drawLine(c, a, col);
			}
		}

		// Draws a rectangle between 2 points
		void drawRectangle(kl::ivec2 a, kl::ivec2 b, kl::color col, bool fill = false) {
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
		void drawCircle(kl::ivec2 p, float r, kl::color col, bool fill = false) {
			if (fill) {
				for (int y = int(p.y - r); y <= int(p.y + r); y++) {
					int x = int(p.x + sqrt(r * r - float(y - p.y) * float(y - p.y)));
					drawLine(kl::ivec2(2 * p.x - x, y), kl::ivec2(x, y), col);
				}
			}
			else {
				for (int i = 0; i < 2 * r; i++) {
					// X run
					int x1 = int(p.x - r + i);
					int y1 = int(p.y + sqrt(r * r - float(x1 - p.x) * float(x1 - p.x)));
					setPixel(kl::ivec2(x1, y1), col);
					setPixel(kl::ivec2(x1, 2 * p.y - y1), col);
					
					// Y run
					int y2 = int(p.y - r + i);
					int x2 = int(p.x + sqrt(r * r - float(y2 - p.y) * float(y2 - p.y)));
					setPixel(kl::ivec2(x2, y2), col);
					setPixel(kl::ivec2(2 * p.x - x2, y2), col);
				}
			}
		}
		// Draws a circle between 1 center and 1 outer point
		void drawCircle(kl::ivec2 a, kl::ivec2 b, kl::color col, bool fill = false) {
			drawCircle(a, kl::vec2(a, b).length(), col, fill);
		}

		// Converts an image to an ASCII frame
		String toASCII(kl::ivec2 frameSize) {
			static const char asciiPixelTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };

			// Calculations
			int pixelWidthIncrement = width / frameSize.x;
			int pixelHeightIncrement = height / frameSize.y;

			// Processing
			std::stringstream frame;
			for (int y = 0; y < frameSize.y; y++) {
				for (int x = 0; x < frameSize.x; x++) {
					// Pixels to grayscale
					kl::color currentPixel = getPixel(kl::ivec2(x * pixelWidthIncrement, y * pixelHeightIncrement));
					int grayScaledPixel = int(currentPixel.r * 0.299 + currentPixel.g * 0.587 + currentPixel.b * 0.114);

					// Grayscaled values to ASCII
					int saturationLevel = int((grayScaledPixel / 255.0) * 9);
					frame << asciiPixelTable[saturationLevel];
				}
				frame << '\n';
			}
			return frame.str();
		}

		// NOT MY IDEA, thank you javidx9 :)
		// Fills the image with random perlin noise
		void genPerlinNoise(int octaveCount, float bias = 1) {
			// Generating random seed array
			std::vector<float> seedArray(width * height);
			for (int i = 0; i < width * height; i++) {
				seedArray[i] = kl::random::FLOAT(0, 1);
			}

			// Generating noise
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					float noise = 0;
					float scale = 1;
					float scaleSum = 0;
					
					for (int i = 0; i < octaveCount; i++) {
						int pitch = !(width >> i) ? 1 : width >> i;
						int x1 = (x / pitch) * pitch;
						int y1 = (y / pitch) * pitch;
						int x2 = (x1 + pitch) % width;
						int y2 = (y1 + pitch) % width;
						float blendX = float(x - x1) / pitch;
						float blendY = float(y - y1) / pitch;
						float sampleT = (1 - blendX) * seedArray[(uint64_t)y1 * width + x1] + blendX * seedArray[(uint64_t)y1 * width + x2];
						float sampleB = (1 - blendX) * seedArray[(uint64_t)y2 * width + x1] + blendX * seedArray[(uint64_t)y2 * width + x2];

						scaleSum += scale;
						noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
						scale /= bias;
					}

					byte grayValue = byte((noise / scaleSum) * 255);
					pixels[(uint64_t)y * width + x].r = grayValue;
					pixels[(uint64_t)y * width + x].g = grayValue;
					pixels[(uint64_t)y * width + x].b = grayValue;
				}
			}
		}

		// Executes a function on each pixel
		void runOnEach(Function<void(kl::color* pixelColor, kl::ivec2 pixelPosition)> toExecute) {
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					toExecute(&pixels[(uint64_t)y * width + x], kl::ivec2(x, y));
				}
			}
		}

	private:
		int width;
		int height;
		std::vector<kl::color> pixels;
	};
}