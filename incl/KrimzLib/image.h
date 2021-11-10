#pragma once


namespace kl {
	class image {
	public:
		// Constructor
		image() {}
		image(size size, color color = {}) {
			SetSize(size);
			FillSolid(color);
		}
		image(const char* fileName) {
			FromFile(fileName);
		}

		// Getters
		uint32 GetWidth() {
			return imageWidth;
		}
		uint32 GetHeight() {
			return imageHeight;
		}
		size GetSize() {
			return { imageWidth, imageHeight };
		}
		color GetPixel(point point) {
			if (point.x >= 0 && point.x < (int)imageWidth && point.y >= 0 && point.y < (int)imageHeight) {
				return imagePixels[point.y * size_t(imageWidth) + point.x];
			}
			return { 0, 0, 0 };
		}
		uint64 GetPixelCount() {
			return imagePixels.size();
		}
		color* GetRawData() {
			return &imagePixels[0];
		}

		// Setters
		void SetWidth(uint32 width) {
			SetSize({ width, imageHeight });
		}
		void SetHeight(uint32 height) {
			SetSize({ imageWidth, height });
		}
		void SetSize(size size) {
			imageWidth = size.width;
			imageHeight = size.height;
			imagePixels.resize(size_t(imageWidth) * size_t(imageHeight));
		}
		void SetPixel(point point, color color) {
			if (point.x >= 0 && point.x < (int)imageWidth && point.y >= 0 && point.y < (int)imageHeight) {
				imagePixels[point.y * size_t(imageWidth) + point.x] = color;
			}
		}

		// Reads an image file and stores it in the image instance
		void FromFile(std::string filePath) {
			// Loads image file
			Gdiplus::Bitmap loadedBitmap(convert::ToWString(filePath).c_str());

			// Checks load status
			if (loadedBitmap.GetLastStatus()) {
				printf("Couldn't load image file \"%s\".", filePath.c_str());
				console::WaitFor(' ', true);
				exit(69);
			}

			// Pixel data loading
			SetSize({ loadedBitmap.GetWidth(), loadedBitmap.GetHeight() });
			for (uint32 y = 0; y < imageHeight; y++) {
				for (uint32 x = 0; x < imageWidth; x++) {
					Gdiplus::Color tempPixel;
					loadedBitmap.GetPixel((int)x, (int)y, &tempPixel);
					SetPixel({ (int)x, (int)y }, { tempPixel.GetR(), tempPixel.GetG() , tempPixel.GetB() });
				}
			}
		}

		// Saves the image to a file
		void ToFile(std::string fileName) {
			static const CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID gifEncoderCLSID = { 0x557cf402, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };

			// Checking the file extension is supported
			const CLSID* formatToUse = nullptr;
			std::string fileExtension = string::GetFileExtension(fileName);
			if (fileExtension == "bmp") {
				formatToUse = &bmpEncoderCLSID;
			}
			else if (fileExtension == "jpg") {
				formatToUse = &jpgEncoderCLSID;
			}
			else if (fileExtension == "gif") {
				formatToUse = &gifEncoderCLSID;
			}
			else if (fileExtension == "png") {
				formatToUse = &pngEncoderCLSID;
			}
			else if (fileExtension == "txt") {
				std::stringstream ss;
				for (uint32 y = 0; y < imageHeight; y++) {
					for (uint32 x = 0; x < imageWidth; x++) {
						ss <<
							x << " " << y << " => " <<
							(int)imagePixels[(uint64)y * imageWidth + x].r << " " <<
							(int)imagePixels[(uint64)y * imageWidth + x].g << " " <<
							(int)imagePixels[(uint64)y * imageWidth + x].b << "\n";
					}
				}
				file::WriteText(fileName, ss.str());
				return;
			}
			else {
				printf("File extension \"%s\" is not supported!\n", fileExtension.c_str());
				return;
			}

			// Pixel data transfer and saving to file
			Gdiplus::Bitmap tempBitmap(imageWidth, imageHeight, PixelFormat24bppRGB);
			for (uint32 y = 0; y < imageHeight; y++) {
				for (uint32 x = 0; x < imageWidth; x++) {
					color tempPixel = GetPixel({ (int)x, (int)y });
					tempBitmap.SetPixel((int)x, (int)y, { tempPixel.r, tempPixel.g, tempPixel.b });
				}
			}
			tempBitmap.Save(convert::ToWString(fileName).c_str(), formatToUse, nullptr);
		}

		// Fils the image with solid color
		void FillSolid(color color) {
			for (uint32 i = 0; i < imagePixels.size(); i++) {
				imagePixels[i] = color;
			}
		}

		// Draws a line between 2 points
		void DrawLine(point a, point b, color col) {
			// Calculations
			int len = max(abs(b.x - a.x), abs(b.y - a.y));
			vec2 incr = { ((double)b.x - (double)a.x) / len, ((double)b.y - (double)a.y) / len };

			// Drawing
			vec2 drawPoint = { (double)a.x, (double)a.y };
			for (int i = 0; i <= len; i++) {
				SetPixel({ (int)drawPoint.x, (int)drawPoint.y }, col);
				drawPoint = drawPoint + incr;
			}
		}

		// Draws a triangle between 3 points
		void DrawTriangle(point a, point b, point c, color col, bool fill = false) {
			if (fill) {
				// Sorting by y
				if (a.y > b.y) {
					point temp = a;
					a = b;
					b = temp;
				}
				if (a.y > c.y) {
					point temp = a;
					a = c;
					c = temp;
				}
				if (b.y > c.y) {
					point temp = b;
					b = c;
					c = temp;
				}

				// Drawing
				for (int y = a.y; y < c.y; y++) {
					DrawLine({ int(kl::math::XofLine((y < b.y) ? a : c, b, y)), y }, { int(kl::math::XofLine(a, c, y)), y }, col);
				}
			}
			else {
				DrawLine(a, b, col);
				DrawLine(b, c, col);
				DrawLine(c, a, col);
			}
		}

		// Draws a rectangle between 2 points
		void DrawRectangle(point a, point b, color col, bool fill = false) {
			if (fill) {
				// Sorting by y
				if (a.y > b.y) {
					point temp = a;
					a = b;
					b = temp;
				}

				// Drawing
				for (int y = a.y; y <= b.y; y++) {
					DrawLine({ a.x, y }, { b.x, y }, col);
				}
			}
			else {
				DrawLine(a, { a.x, b.y }, col);
				DrawLine(a, { b.x, a.y }, col);
				DrawLine(b, { a.x, b.y }, col);
				DrawLine(b, { b.x, a.y }, col);
			}
		}

		// Draws a circle with the given center point and radius
		void DrawCircle(point p, double r, color col, bool fill = false) {
			if (fill) {
				for (int y = int(p.y - r); y <= int(p.y + r); y++) {
					int x = int((double)p.x + sqrt(r * r - double(y - p.y) * double(y - p.y)));
					DrawLine({ 2 * p.x - x, y }, { x, y }, col);
				}
			}
			else {
				for (int i = 0; i < 2 * r; i++) {
					// X run
					int x1 = int(p.x - r + i);
					int y1 = int((double)p.y + sqrt(r * r - double(x1 - p.x) * double(x1 - p.x)));
					SetPixel({ x1, y1 }, col);
					SetPixel({ x1, 2 * p.y - y1 }, col);
					
					// Y run
					int y2 = int(p.y - r + i);
					int x2 = int((double)p.x + sqrt(r * r - double(y2 - p.y) * double(y2 - p.y)));
					SetPixel({ x2, y2 }, col);
					SetPixel({ 2 * p.x - x2, y2 }, col);
				}
			}
		}
		// Draws a circle between 1 center and 1 outer point
		void DrawCircle(point a, point b, color col, bool fill = false) {
			DrawCircle(a, vec2(a, b).Length(), col, fill);
		}

		// Converts an image to an ASCII frame
		std::string ToASCII(size frameSize) {
			static const char asciiPixelTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };

			// Calculations
			uint32 pixelWidthIncrement = imageWidth / frameSize.width;
			uint32 pixelHeightIncrement = imageHeight / frameSize.height;

			// Processing
			std::stringstream frame;
			for (uint32 y = 0; y < frameSize.height; y++) {
				for (uint32 x = 0; x < frameSize.width; x++) {
					// Pixels to grayscale
					color currentPixel = GetPixel({ int(x * pixelWidthIncrement), int(y * pixelHeightIncrement) });
					uint32 grayScaledPixel = (uint32)(currentPixel.r * 0.299 + currentPixel.g * 0.587 + currentPixel.b * 0.114);

					// Grayscaled values to ASCII
					uint32 saturationLevel = (uint32)((grayScaledPixel / 255.0) * 9);
					frame << asciiPixelTable[saturationLevel];
				}
				frame << '\n';
			}
			return frame.str();
		}

		// Executes a function on each pixel
		void RunOnEach(std::function<void(kl::color* pixelColor, kl::point pixelPosition)> toExecute) {
			for (uint32 y = 0; y < imageHeight; y++) {
				for (uint32 x = 0; x < imageWidth; x++) {
					toExecute(&imagePixels[uint64(y * imageWidth + x)], kl::point(x, y));
				}
			}
		}

	protected:
		// Initalises gdiplus
		static void InitGdiPlus() {
			if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr)) {
				printf("Failed to initalise gdiplus\n");
				console::WaitFor(' ', true);
				exit(69);
			}
		}

		// Uninitalises gdiplus
		static void UninitGdiPlus() {
			Gdiplus::GdiplusShutdown(gdiplusToken);
		}

	private:
		static ULONG_PTR gdiplusToken;
		static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		uint32 imageWidth = 0;
		uint32 imageHeight = 0;
		std::vector<color> imagePixels = {};
	};
	ULONG_PTR image::gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput image::gdiplusStartupInput = {};
}