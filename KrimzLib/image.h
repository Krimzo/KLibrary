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
		image(const wchar_t* fileName) {
			FromFile(fileName);
		}

		// Getters
		size GetSize() {
			return { width, height };
		}
		int GetWidth() {
			return width;
		}
		int GetHeight() {
			return height;
		}
		size_t GetLenght() {
			return pixels.size();
		}
		color GetPixel(point point) {
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
				return pixels[point.y * size_t(width) + point.x];
			}
			return { 0, 0, 0 };
		}
		color* GetRawData() {
			return &pixels[0];
		}

		// Setters
		void SetSize(size size) {
			width = size.width;
			height = size.height;
			pixels.resize(size_t(width) * size_t(height));
		}
		void SetWidth(int width) {
			SetSize({ width, height });
		}
		void SetHeight(int height) {
			SetSize({ width, height });
		}
		void SetPixel(point point, color color) {
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height) {
				pixels[point.y * size_t(width) + point.x] = color;
			}
		}
		
		// Reads an image file and stores it in the image instance
		void FromFile(std::wstring filePath) {
			if (gdipInitialised) {
				// Loads image file
				Gdiplus::Bitmap loadedBitmap(filePath.c_str());

				// Checks load status
				if (loadedBitmap.GetLastStatus()) {
					wprintf(L"Couldn't load image file \"%s\".", filePath.c_str());
					kl::console::WaitFor(' ', true);
					exit(69);
				}

				// Saves data
				Gdiplus::BitmapData bitmapData = {};
				Gdiplus::Rect rect(0, 0, loadedBitmap.GetWidth(), loadedBitmap.GetHeight());
				loadedBitmap.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat24bppRGB, &bitmapData);
				byte* rawBitmapData = (byte*)bitmapData.Scan0;
				if (rawBitmapData) {
					SetSize({ (int)loadedBitmap.GetWidth(), (int)loadedBitmap.GetHeight() });
					memcpy(GetRawData(), rawBitmapData, pixels.size() * 3);
				}
			}
		}

		// Saves the image to a file
		void ToFile(std::wstring fileName) {
			if (gdipInitialised) {
				// Choosing the image format
				const CLSID* formatToUse = NULL;
				std::wstring fileExtension = string::GetFileExtension(fileName);
				if (fileExtension == L".bmp") {
					formatToUse = &bmpEncoderCLSID;
				}
				else if (fileExtension == L".jpg") {
					formatToUse = &jpgEncoderCLSID;
				}
				else if (fileExtension == L".gif") {
					formatToUse = &gifEncoderCLSID;
				}
				else if (fileExtension == L".png") {
					formatToUse = &pngEncoderCLSID;
				}
				else {
					wprintf(L"File extension \"%s\" is not supported!\n", fileExtension.c_str());
					return;
				}

				// Gdiplus bitmap setup
				Gdiplus::BitmapData bitmapData = {};
				Gdiplus::Rect rect(0, 0, width, height);
				Gdiplus::Bitmap tempBitmap(width, height, PixelFormat24bppRGB);
				tempBitmap.LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &bitmapData);
				byte* rawBitmapData = (byte*)bitmapData.Scan0;

				// Data copy
				if (rawBitmapData) {
					memcpy(rawBitmapData, GetRawData(), pixels.size() * 3);
					tempBitmap.UnlockBits(&bitmapData);
					tempBitmap.Save(fileName.c_str(), formatToUse, NULL);
				}
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

		// Converts an image to an ASCII frame
		std::string ToASCII(size frameSize) {
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

	private:
		static bool gdipInitialised;
		static ULONG_PTR gdiplusToken;
		static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		static const CLSID bmpEncoderCLSID;
		static const CLSID jpgEncoderCLSID;
		static const CLSID gifEncoderCLSID;
		static const CLSID pngEncoderCLSID;
		static char asciiPixelTable[10];
		int width = 0;
		int height = 0;
		std::vector<color> pixels = {};
	};
	bool image::gdipInitialised = false;
	ULONG_PTR image::gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput image::gdiplusStartupInput = {};
	const CLSID image::bmpEncoderCLSID = { 0x557cf400, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
	const CLSID image::jpgEncoderCLSID = { 0x557cf401, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
	const CLSID image::gifEncoderCLSID = { 0x557cf402, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
	const CLSID image::pngEncoderCLSID = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } };
	char image::asciiPixelTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
}