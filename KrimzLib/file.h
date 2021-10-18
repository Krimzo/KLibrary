#pragma once


namespace kl
{
	class file
	{
	public:
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
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
			Gdiplus::Bitmap* loadedBitmap = Gdiplus::Bitmap::FromFile(imagePath.c_str());

			// Checks load status
			int lastBitmapStatus = loadedBitmap->GetLastStatus();
			if (lastBitmapStatus)
			{
				std::wcout << "Couldn't load image file \"" << imagePath << "\", status: " << lastBitmapStatus << std::endl;
				kl::console::WaitFor(' ', true);
				exit(69);
			}

			// Saves data
			bitmap tempBitmap({ (int)loadedBitmap->GetWidth(), (int)loadedBitmap->GetHeight() });
			for (int y = 0; y < tempBitmap.GetHeight(); y++)
			{
				for (int x = 0; x < tempBitmap.GetWidth(); x++)
				{
					Gdiplus::Color tempPixel;
					loadedBitmap->GetPixel(x, y, &tempPixel);
					tempBitmap.SetPixel({ x, y }, { tempPixel.GetR(), tempPixel.GetG(), tempPixel.GetB(), 255 });
				}
			}

			// Clears memory
			delete loadedBitmap;
			Gdiplus::GdiplusShutdown(gdiplusToken);

			// Return created bitmap
			return tempBitmap;
		}

		// Deletes a given file
		static void Delete(std::wstring filePath)
		{
			_wremove(filePath.c_str());
		}

		// Extract the video frames and saves them as .jpg files
		static void ExtractFrames(std::wstring videoPath, std::wstring outputPath)
		{
			if (outputPath.back() != '/')
				outputPath.push_back('/');

			std::wstring command = L"ffmpeg -i " + videoPath + L" " + outputPath + L"frame%d.jpg";
			_wsystem(command.c_str());

			kl::console::Print("Frames extracted!\n", kl::constant::colorOrange);
		}
	};
}
