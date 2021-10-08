#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <gdiplus.h>
#include "KrimzLib/types.h"
#pragma comment (lib, "gdiplus.lib")

namespace kl
{
	class file
	{
	public:
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
			if (fileStream.is_open())
			{
				textBuffer << fileStream.rdbuf();
			}
			else
			{
				std::wcout << "Couldn't load text file \"" << filePath << "\"" << std::endl;
				char iHateWarnings = getchar();
				exit(69);
			}
			fileStream.close();
			return textBuffer.str();
		}

		// Writes text to a text file
		static void WriteText(std::wstring filePath, std::string& data)
		{
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}

		// Appends text to a text file
		static void AppendText(std::wstring filePath, std::string& data, int position = -1)
		{
			std::fstream fileStream(filePath, std::ios::in | std::ios::out);
			if (position < 0)
			{
				fileStream.seekp(0, std::ios_base::end);
			}
			else
			{
				fileStream.seekp(position);
			}
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
				char iHateWarnings = getchar();
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
		static void WriteBytes(std::wstring filePath, bytes& data)
		{
			std::ofstream fileStream(filePath, std::ios::binary);
			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}

		// Appends bytes to a file
		static void AppendBytes(std::wstring filePath, bytes& data, int position = -1)
		{
			std::fstream fileStream(filePath, std::ios::in | std::ios::out | std::ios::binary);
			if (position < 0)
			{
				fileStream.seekp(0, std::ios_base::end);
			}
			else
			{
				fileStream.seekp(position);
			}
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
				char iHateWarnings = getchar();
				exit(69);
			}

			// Saves data
			bitmap tempBitmap(loadedBitmap->GetWidth(), loadedBitmap->GetHeight());
			for (int y = 0; y < tempBitmap.GetHeight(); y++)
			{
				for (int x = 0; x < tempBitmap.GetWidth(); x++)
				{
					Gdiplus::Color tempPixel;
					loadedBitmap->GetPixel(x, y, &tempPixel);
					tempBitmap.SetPixel(x, y, { tempPixel.GetR(), tempPixel.GetG(), tempPixel.GetB(), 255 });
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
	};
}
