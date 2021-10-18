#pragma once


namespace kl
{
	class file
	{
	public:
		// Initalises gdiplus
		static void InitGdiplus()
		{
			if (!initialised)
			{
				Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
				initialised = true;
			}
		}

		// Uninitalises gdiplus
		static void UninitGdiplus()
		{
			if (initialised)
			{
				Gdiplus::GdiplusShutdown(gdiplusToken);
				initialised = false;
			}
		}

		// Creates a file
		static void Create(std::wstring filePath)
		{
			std::ofstream fileStream(filePath);
			fileStream.close();
		}

		// Opens a file with default program
		static void OpenWithDefault(std::wstring filePath)
		{
			_wsystem(filePath.c_str());
		}

		// Opens a file with notepad
		static void OpenWithNotepad(std::wstring filePath)
		{
			_wsystem((L"notepad " + filePath).c_str());
		}

		// Opens a file with visual studio code
		static void OpenWithVSCode(std::wstring filePath)
		{
			_wsystem((L"code " + filePath).c_str());
		}

		// Returns a string from a given text file
		static std::string ReadText(std::wstring filePath)
		{
			std::ifstream fileStream(filePath);
			std::stringstream textBuffer;
			if (!fileStream.is_open())
			{
				std::wcout << "Couldn't load text file \"" << filePath << "\"" << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}
			textBuffer << fileStream.rdbuf();
			fileStream.close();
			return textBuffer.str();
		}

		// Writes text to a text file
		static void WriteText(std::string& data, std::wstring filePath)
		{
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}
		static void WriteText(std::string&& data, std::wstring filePath)
		{
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}

		// Appends text to a text file
		static void AppendText(std::string& data, std::wstring filePath, int position = -1)
		{
			std::fstream fileStream(filePath, std::ios::in | std::ios::out);
			if (!fileStream.is_open())
			{
				std::wcout << "Couldn't load text file \"" << filePath << "\"" << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}

			if (position < 0)
				fileStream.seekp(0, std::ios_base::end);
			else
				fileStream.seekp(position);

			fileStream << data;
			fileStream.close();
		}
		static void AppendText(std::string&& data, std::wstring filePath, int position = -1)
		{
			std::fstream fileStream(filePath, std::ios::in | std::ios::out);
			if (!fileStream.is_open())
			{
				std::wcout << "Couldn't load text file \"" << filePath << "\"" << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}

			if (position < 0)
				fileStream.seekp(0, std::ios_base::end);
			else
				fileStream.seekp(position);

			fileStream << data;
			fileStream.close();
		}

		// Returns a filedata from a given file
		static bytes ReadBytes(std::wstring filePath)
		{
			std::ifstream fileStream(filePath, std::ios::binary);
			if (!fileStream.is_open())
			{
				std::wcout << "Couldn't load file \"" << filePath << "\"" << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}
			fileStream.seekg(0, std::ios::end);
			size_t fileLen = fileStream.tellg();
			fileStream.seekg(0, std::ios::beg);
			bytes byteArray(fileLen);
			fileStream.read((char*)&byteArray[0], fileLen);
			fileStream.close();
			return byteArray;
		}

		// Writes bytes to a file
		static void WriteBytes(bytes& data, std::wstring filePath)
		{
			std::ofstream fileStream(filePath, std::ios::binary);
			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}
		static void WriteBytes(bytes&& data, std::wstring filePath)
		{
			std::ofstream fileStream(filePath, std::ios::binary);
			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}

		// Appends bytes to a file
		static void AppendBytes(bytes& data, std::wstring filePath, int position = -1)
		{
			std::fstream fileStream(filePath, std::ios::in | std::ios::out | std::ios::binary);
			if (!fileStream.is_open())
			{
				std::wcout << "Couldn't load file \"" << filePath << "\"" << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}

			if (position < 0)
				fileStream.seekp(0, std::ios_base::end);
			else
				fileStream.seekp(position);

			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}
		static void AppendBytes(bytes&& data, std::wstring filePath, int position = -1)
		{
			std::fstream fileStream(filePath, std::ios::in | std::ios::out | std::ios::binary);
			if (!fileStream.is_open())
			{
				std::wcout << "Couldn't load file \"" << filePath << "\"" << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}

			if (position < 0)
				fileStream.seekp(0, std::ios_base::end);
			else
				fileStream.seekp(position);

			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}

		// Returns a bitmap from the given image file
		static bitmap ReadPixels(std::wstring imagePath)
		{
			// Loads image file
			Gdiplus::Bitmap loadedBitmap(imagePath.c_str());

			// Checks load status
			int lastBitmapStatus = loadedBitmap.GetLastStatus();
			if (lastBitmapStatus)
			{
				std::wcout << "Couldn't load image file \"" << imagePath << "\", status: " << lastBitmapStatus << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}

			// Saves data
			bitmap tempBitmap({ (int)loadedBitmap.GetWidth(), (int)loadedBitmap.GetHeight() });
			for (int y = 0; y < tempBitmap.GetHeight(); y++)
			{
				for (int x = 0; x < tempBitmap.GetWidth(); x++)
				{
					Gdiplus::Color tempPixel;
					loadedBitmap.GetPixel(x, y, &tempPixel);
					tempBitmap.SetPixel({ x, y }, { tempPixel.GetR(), tempPixel.GetG(), tempPixel.GetB(), tempPixel.GetA() });
				}
			}

			// Return created bitmap
			return tempBitmap;
		}

		// Deletes a given file
		static void Delete(std::wstring filePath)
		{
			_wremove(filePath.c_str());
		}

	private:
		static bool initialised;
		static ULONG_PTR gdiplusToken;
		static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	};
	bool file::initialised = false;
	ULONG_PTR file::gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput file::gdiplusStartupInput = {};
}
