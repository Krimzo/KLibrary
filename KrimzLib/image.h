#pragma once


namespace kl {
	class image {
	public:
		// Initalises gdiplus
		static void InitGdiPlus() {
			if (!gdipInitialised) {
				Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
				gdipInitialised = true;
			}
		}

		// Uninitalises gdiplus
		static void UninitGdiPlus() {
			if (gdipInitialised) {
				Gdiplus::GdiplusShutdown(gdiplusToken);
				gdipInitialised = false;
			}
		}

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
			if (gdipInitialised) {
				// Loads image file
				Gdiplus::Bitmap loadedBitmap(convert::ToWString(filePath).c_str());

				// Checks load status
				if (loadedBitmap.GetLastStatus()) {
					printf("Couldn't load image file \"%s\".", filePath.c_str());
					console::WaitFor(' ', true);
					exit(69);
				}

				// Data saving
				SetSize({ (int)loadedBitmap.GetWidth(), (int)loadedBitmap.GetHeight() });
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						Gdiplus::Color tempPixel;
						loadedBitmap.GetPixel(x, y, &tempPixel);
						SetPixel({ x, y }, { tempPixel.GetR(), tempPixel.GetG() , tempPixel.GetB() });
					}
				}
			}
		}

		// Saves the image to a file
		void ToFile(std::string fileName) {
			static const CLSID bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID gifEncoderCLSID = { 0x557cf402, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
			static const CLSID pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };

			if (gdipInitialised) {
				// Checking the file extension
				const CLSID* formatToUse = NULL;
				std::string fileExtension = string::GetFileExtension(fileName);
				if (fileExtension == ".bmp") {
					formatToUse = &bmpEncoderCLSID;
				}
				else if (fileExtension == ".jpg") {
					formatToUse = &jpgEncoderCLSID;
				}
				else if (fileExtension == ".gif") {
					formatToUse = &gifEncoderCLSID;
				}
				else if (fileExtension == ".png") {
					formatToUse = &pngEncoderCLSID;
				}
				else {
					printf("File extension \"%s\" is not supported!\n", fileExtension.c_str());
					return;
				}

				// Data transfer and saving to file
				Gdiplus::Bitmap tempBitmap(width, height, PixelFormat24bppRGB);
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						color tempPixel = GetPixel({ x, y });
						tempBitmap.SetPixel(x, y, { tempPixel.r, tempPixel.g, tempPixel.b });
					}
				}
				tempBitmap.Save(convert::ToWString(fileName).c_str(), formatToUse, NULL);
			}
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
		void DrawLine(point a, point b, color c = { 255, 255, 255 }) {
			// Calculations
			int len = max(abs(b.x - a.x), abs(b.y - a.y));
			vec2 incr = { double(b.x - a.x) / len, double(b.y - a.y) / len };

			// Drawing
			vec2 drawPoint = { (double)a.x, (double)a.y };
			for (int i = 0; i < len; i++) {
				SetPixel({ (int)drawPoint.x, (int)drawPoint.y }, c);
				drawPoint = drawPoint + incr;
			}
		}

		// Draws a rectangle between 2 points
		void DrawRect(point a, point b, color c = { 255, 255, 255 }, bool fill = false) {
			if (fill) {
				
			}
			else {
				DrawLine(a, { a.x, b.y }, c);
				DrawLine(a, { b.x, a.y }, c);
				DrawLine(b, { a.x, b.y }, c);
				DrawLine(b, { b.x, a.y }, c);
			}
		}

		// Prints the image to the console
		void ToConsole() {
			// Calculations
			size consoleSize = { console::GetSize().width, console::GetSize().height - 1 };
			int pixelWidthIncrement = width / consoleSize.width;
			int pixelHeightIncrement = height / consoleSize.height;

			// Printing
			for (int y = 0; y < consoleSize.height; y++) {
				for (int x = 0; x < consoleSize.width; x++) {
					console::PrintCell(GetPixel({ x * pixelWidthIncrement, y * pixelHeightIncrement }));
				}
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

		static bool gdipInitialised;
		static ULONG_PTR gdiplusToken;
		static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	};
	bool image::gdipInitialised = false;
	ULONG_PTR image::gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput image::gdiplusStartupInput = {};
}