#pragma once


namespace kl
{
	class image
	{
	public:
		// Initalises gdiplus
		static void InitGdiPlus()
		{
			if (!initialised)
			{
				Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
				initialised = true;
			}
		}

		// Uninitalises gdiplus
		static void UninitGdiPlus()
		{
			if (initialised)
			{
				Gdiplus::GdiplusShutdown(gdiplusToken);
				initialised = false;
			}
		}

		// Returns an image from the given image file
		static image FromFile(std::wstring filePath)
		{
			if (initialised)
			{
				// Loads image file
				Gdiplus::Bitmap loadedBitmap(filePath.c_str());

				// Checks load status
				int lastBitmapStatus = loadedBitmap.GetLastStatus();
				if (lastBitmapStatus)
				{
					std::wcout << "Couldn't load image file \"" << filePath << "\", status: " << lastBitmapStatus << std::endl;
					kl::console::WaitFor(' ', true);
					exit(69);
				}

				// Saves data
				image tempImage({ (int)loadedBitmap.GetWidth(), (int)loadedBitmap.GetHeight() });
				for (int y = 0; y < tempImage.GetHeight(); y++)
				{
					for (int x = 0; x < tempImage.GetWidth(); x++)
					{
						Gdiplus::Color tempPixel = {};
						loadedBitmap.GetPixel(x, y, &tempPixel);
						tempImage.SetPixel({ x, y }, { tempPixel.GetR(), tempPixel.GetG(), tempPixel.GetB(), tempPixel.GetA() });
					}
				}

				// Return created image
				return tempImage;
			}

			// Return an empty image
			return { { 0, 0 } };
		}

		// Constructor
		image(size size, color color = { 0, 0, 0, 255 })
		{
			width = size.width;
			height = size.height;
			pixels.resize(size_t(width) * size_t(height));
			FillSolid(color);
		}

		// Getters
		int GetWidth()
		{
			return width;
		}
		int GetHeight()
		{
			return height;
		}
		size GetSize()
		{
			return { width, height };
		}
		size_t GetLenght()
		{
			return pixels.size();
		}
		color GetPixel(point point)
		{
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height)
				return pixels[point.y * size_t(width) + point.x];
			return { 0, 0, 0 };
		}
		color* GetPixelData()
		{
			return &pixels[0];
		}

		// Setters
		void SetWidth(int width)
		{
			this->width = width;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetHeight(int height)
		{
			this->height = height;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetSize(size size)
		{
			width = size.width;
			height = size.height;
			pixels.resize(size_t(width) * size_t(height));
		}
		void SetPixel(point point, color color)
		{
			if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height)
				pixels[point.y * size_t(width) + point.x] = color;
		}

		// Fils the image with solid color
		void FillSolid(color color)
		{
			std::fill(pixels.begin(), pixels.end(), color);
		}

		// Resets the byte values
		void FastClear(byte value)
		{
			memset(&pixels[0], value, pixels.size() * 4);
		}

		// Flips red and blue color channels
		void FlipRB()
		{
			for (int i = 0; i < pixels.size(); i++)
			{
				byte tempByte = pixels[i].r;
				pixels[i].r = pixels[i].b;
				pixels[i].b = tempByte;
			}
		}

		// Converts an image to an ASCII frame
		std::string ToASCII(size frameSize)
		{
			// Calculations
			int pixelWidthIncrement = width / frameSize.width;
			int pixelHeightIncrement = height / frameSize.height;

			// Processing
			std::stringstream frame;
			for (int y = 0; y < frameSize.height; y++)
			{
				for (int x = 0; x < frameSize.width; x++)
				{
					// Pixels to grayscale
					color currentPixel = this->GetPixel({ x * pixelWidthIncrement, y * pixelHeightIncrement });
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
		static bool initialised;
		static ULONG_PTR gdiplusToken;
		static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		static char asciiPixelTable[10];
		int width = 0;
		int height = 0;
		std::vector<color> pixels = {};
	};
	bool image::initialised = false;
	ULONG_PTR image::gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput image::gdiplusStartupInput = {};
	char image::asciiPixelTable[10] = {'@', '%', '#', 'x', '+', '=', ':', '-', '.', ' '};
}