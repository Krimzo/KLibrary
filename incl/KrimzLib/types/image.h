#pragma once


namespace kl {
	class image {
	public:
		// Constructor
		image() {
			setSize(kl::size(0, 0));
		}
		image(kl::size size, kl::color color = {}) {
			setSize(size);
			fillSolid(color);
		}
		image(const char* fileName) {
			fromFile(fileName);
		}

		// Getters
		kl::uint32 getWidth() {
			return imageWidth;
		}
		kl::uint32 getHeight() {
			return imageHeight;
		}
		kl::size getSize() {
			return kl::size(imageWidth, imageHeight);
		}
		kl::color getPixel(kl::point point) {
			if (point.x >= 0 && point.x < int(imageWidth) && point.y >= 0 && point.y < int(imageHeight)) {
				return imagePixels[point.y * imageWidth + point.x];
			}
			return kl::color();
		}
		kl::uint64 getPixelCount() {
			return imagePixels.size();
		}
		kl::byte* pointer() {
			return (kl::byte*)&imagePixels[0];
		}

		// Setters
		void setWidth(kl::uint32 width) {
			setSize(kl::size(width, imageHeight));
		}
		void setHeight(kl::uint32 height) {
			setSize(kl::size(imageWidth, height));
		}
		void setSize(kl::size size) {
			imageWidth = size.width;
			imageHeight = size.height;
			imagePixels.resize(imageWidth * imageHeight);
		}
		void setPixel(kl::point point, kl::color color) {
			if (point.x >= 0 && point.x < int(imageWidth) && point.y >= 0 && point.y < int(imageHeight)) {
				imagePixels[point.y * imageWidth + point.x] = color;
			}
		}

		// Reads an image file and stores it in the image instance
		void fromFile(std::string filePath) {
			// Loads image file
			Gdiplus::Bitmap loadedBitmap(kl::convert::toWString(filePath).c_str());

			// Checks load status
			if (loadedBitmap.GetLastStatus()) {
				printf("Couldn't load image file \"%s\".", filePath.c_str());
				exit(69);
			}

			// Pixel data loading
			setSize(kl::size(loadedBitmap.GetWidth(), loadedBitmap.GetHeight()));
			for (kl::uint32 y = 0; y < imageHeight; y++) {
				for (kl::uint32 x = 0; x < imageWidth; x++) {
					Gdiplus::Color tempPixel;
					loadedBitmap.GetPixel((int)x, (int)y, &tempPixel);
					setPixel(kl::point(x, y), kl::color(tempPixel.GetR(), tempPixel.GetG() , tempPixel.GetB()));
				}
			}
		}

		// Saves the image to a file
		void toFile(std::string fileName) {
			static const CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };

			// Checking the file extension is supported
			const CLSID* formatToUse = nullptr;
			std::string fileExtension = kl::string::getFileExtension(fileName);
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
				for (kl::uint32 y = 0; y < imageHeight; y++) {
					for (kl::uint32 x = 0; x < imageWidth; x++) {
						ss <<
							x << " " << y << " => " <<
							int(imagePixels[y * imageWidth + x].r) << " " <<
							int(imagePixels[y * imageWidth + x].g) << " " <<
							int(imagePixels[y * imageWidth + x].b) << "\n";
					}
				}
				kl::file::writeText(fileName, ss.str());
				return;
			}
			else {
				printf("File extension \"%s\" is not supported!\n", fileExtension.c_str());
				return;
			}

			// Pixel data transfer and saving to file
			Gdiplus::Bitmap tempBitmap(imageWidth, imageHeight, PixelFormat24bppRGB);
			for (kl::uint32 y = 0; y < imageHeight; y++) {
				for (kl::uint32 x = 0; x < imageWidth; x++) {
					kl::color tempPixel = getPixel(kl::point(x, y));
					tempBitmap.SetPixel(int(x), int(y), Gdiplus::Color(tempPixel.r, tempPixel.g, tempPixel.b));
				}
			}
			tempBitmap.Save(kl::convert::toWString(fileName).c_str(), formatToUse, nullptr);
		}

		// Fils the image with solid color
		void fillSolid(kl::color color) {
			for (kl::uint32 i = 0; i < imagePixels.size(); i++) {
				imagePixels[i] = color;
			}
		}

		// Flips the pixel on x axis
		void flipHorizontal() {
			const int halfWidth = imageWidth / 2;
			for (int y = 0; y < imageHeight; y++) {
				for (int x = 0; x < halfWidth; x++) {
					kl::color tempPixel = imagePixels[y * imageWidth + x];
					imagePixels[y * imageWidth + x] = imagePixels[y * imageWidth + (imageWidth - 1 - x)];
					imagePixels[y * imageWidth + (imageWidth - 1 - x)] = tempPixel;
				}
			}
		}

		// Flips the pixel on y axis
		void flipVertical() {
			const int halfHeight = imageHeight / 2;
			for (int x = 0; x < imageWidth; x++) {
				for (int y = 0; y < halfHeight; y++) {
					kl::color tempPixel = imagePixels[y * imageWidth + x];
					imagePixels[y * imageWidth + x] = imagePixels[(imageHeight - 1 - y) * imageWidth + x];
					imagePixels[(imageHeight - 1 - y) * imageWidth + x] = tempPixel;
				}
			}
		}

		// Draws a line between 2 points
		void drawLine(kl::point a, kl::point b, kl::color col) {
			// Calculations
			int len = std::max(abs(b.x - a.x), abs(b.y - a.y));
			kl::vec2 incr((float(b.x) - a.x) / len, (float(b.y) - a.y) / len);

			// Drawing
			kl::vec2 drawPoint((float)a.x, (float)a.y);
			for (int i = 0; i <= len; i++) {
				setPixel(kl::point((int)drawPoint.x, (int)drawPoint.y), col);
				drawPoint = drawPoint + incr;
			}
		}

		// Draws a triangle between 3 points
		void drawTriangle(kl::point a, kl::point b, kl::point c, kl::color col, bool fill = false) {
			if (fill) {
				// Sorting by y
				if (a.y > b.y) {
					kl::point temp = a;
					a = b;
					b = temp;
				}
				if (a.y > c.y) {
					kl::point temp = a;
					a = c;
					c = temp;
				}
				if (b.y > c.y) {
					kl::point temp = b;
					b = c;
					c = temp;
				}

				// Drawing
				for (int y = a.y; y < c.y; y++) {
					drawLine(kl::point((int)kl::math::lineX((y < b.y) ? a : c, b, (float)y), y), kl::point((int)kl::math::lineX(a, c, (float)y), y), col);
				}
			}
			else {
				drawLine(a, b, col);
				drawLine(b, c, col);
				drawLine(c, a, col);
			}
		}

		// Draws a rectangle between 2 points
		void drawRectangle(kl::point a, kl::point b, kl::color col, bool fill = false) {
			if (fill) {
				// Sorting by y
				if (a.y > b.y) {
					kl::point temp = a;
					a = b;
					b = temp;
				}

				// Drawing
				for (int y = a.y; y <= b.y; y++) {
					drawLine(kl::point(a.x, y), kl::point(b.x, y), col);
				}
			}
			else {
				drawLine(a, kl::point(a.x, b.y), col);
				drawLine(a, kl::point(b.x, a.y), col);
				drawLine(b, kl::point(a.x, b.y), col);
				drawLine(b, kl::point(b.x, a.y), col);
			}
		}

		// Draws a circle with the given center point and radius
		void drawCircle(kl::point p, float r, kl::color col, bool fill = false) {
			if (fill) {
				for (int y = int(p.y - r); y <= int(p.y + r); y++) {
					int x = int(p.x + sqrt(r * r - float(y - p.y) * float(y - p.y)));
					drawLine(kl::point(2 * p.x - x, y), kl::point(x, y), col);
				}
			}
			else {
				for (int i = 0; i < 2 * r; i++) {
					// X run
					int x1 = int(p.x - r + i);
					int y1 = int(p.y + sqrt(r * r - float(x1 - p.x) * float(x1 - p.x)));
					setPixel(kl::point(x1, y1), col);
					setPixel(kl::point(x1, 2 * p.y - y1), col);
					
					// Y run
					int y2 = int(p.y - r + i);
					int x2 = int(p.x + sqrt(r * r - float(y2 - p.y) * float(y2 - p.y)));
					setPixel(kl::point(x2, y2), col);
					setPixel(kl::point(2 * p.x - x2, y2), col);
				}
			}
		}
		// Draws a circle between 1 center and 1 outer point
		void drawCircle(kl::point a, kl::point b, kl::color col, bool fill = false) {
			drawCircle(a, kl::vec2(a, b).length(), col, fill);
		}

		// Converts an image to an ASCII frame
		std::string toASCII(kl::size frameSize) {
			static const char asciiPixelTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };

			// Calculations
			kl::uint32 pixelWidthIncrement = imageWidth / frameSize.width;
			kl::uint32 pixelHeightIncrement = imageHeight / frameSize.height;

			// Processing
			std::stringstream frame;
			for (kl::uint32 y = 0; y < frameSize.height; y++) {
				for (kl::uint32 x = 0; x < frameSize.width; x++) {
					// Pixels to grayscale
					kl::color currentPixel = getPixel(kl::point(x * pixelWidthIncrement, y * pixelHeightIncrement));
					kl::uint32 grayScaledPixel = kl::uint32(currentPixel.r * 0.299 + currentPixel.g * 0.587 + currentPixel.b * 0.114);

					// Grayscaled values to ASCII
					kl::uint32 saturationLevel = kl::uint32((grayScaledPixel / 255.0) * 9);
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
			std::vector<float> seedArray(imageWidth * imageHeight);
			for (kl::uint32 i = 0; i < imageWidth * imageHeight; i++) {
				seedArray[i] = kl::random::getFloat(0, 1);
			}

			// Generating noise
			for (kl::uint32 y = 0; y < imageHeight; y++) {
				for (kl::uint32 x = 0; x < imageWidth; x++) {
					float noise = 0;
					float scale = 1;
					float scaleSum = 0;
					
					for (int i = 0; i < octaveCount; i++) {
						int pitch = !(imageWidth >> i) ? 1 : imageWidth >> i;
						int x1 = (x / pitch) * pitch;
						int y1 = (y / pitch) * pitch;
						int x2 = (x1 + pitch) % imageWidth;
						int y2 = (y1 + pitch) % imageWidth;
						float blendX = float(x - x1) / pitch;
						float blendY = float(y - y1) / pitch;
						float sampleT = (1 - blendX) * seedArray[y1 * imageWidth + x1] + blendX * seedArray[y1 * imageWidth + x2];
						float sampleB = (1 - blendX) * seedArray[y2 * imageWidth + x1] + blendX * seedArray[y2 * imageWidth + x2];

						scaleSum += scale;
						noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
						scale /= bias;
					}

					byte grayValue = byte((noise / scaleSum) * 255);
					imagePixels[y * imageWidth + x].r = grayValue;
					imagePixels[y * imageWidth + x].g = grayValue;
					imagePixels[y * imageWidth + x].b = grayValue;
				}
			}
		}

		// Executes a function on each pixel
		void runOnEach(std::function<void(kl::color* pixelColor, kl::point pixelPosition)> toExecute) {
			for (kl::uint32 y = 0; y < imageHeight; y++) {
				for (kl::uint32 x = 0; x < imageWidth; x++) {
					toExecute(&imagePixels[y * imageWidth + x], kl::point(x, y));
				}
			}
		}

	protected:
		// Initalises gdiplus
		static void initGdiPlus() {
			if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr)) {
				printf("Failed to initalise gdiplus\n");
				kl::console::waitFor(' ', true);
				exit(69);
			}
		}

		// Uninitalises gdiplus
		static void uninitGdiPlus() {
			Gdiplus::GdiplusShutdown(gdiplusToken);
		}

	private:
		static ULONG_PTR gdiplusToken;
		static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		kl::uint32 imageWidth;
		kl::uint32 imageHeight;
		std::vector<kl::color> imagePixels;
	};
	ULONG_PTR image::gdiplusToken = NULL;
	Gdiplus::GdiplusStartupInput image::gdiplusStartupInput = {};
}