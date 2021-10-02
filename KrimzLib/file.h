#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <gdiplus.h>
#include "KrimzLib/types.h"


namespace kl
{
	class file
	{
	public:
		// Returns a string from a given text file
		static std::string GetText(std::wstring fileName)
		{
			std::stringstream textBuffer;
			std::ifstream fileStream(fileName);
			if (fileStream.is_open())
			{
				textBuffer << fileStream.rdbuf();
			}
			else
			{
				std::wcout << "Couldn't load text file \"" << fileName << "\"" << std::endl;
				char iHateWarnings = getchar();
				exit(69);
			}
			fileStream.close();
			return textBuffer.str();
		}

		// Returns a filedata from a given file
		static filedata GetBytes(std::wstring fileName)
		{
			filedata tempFileData = { fileName };
			std::ifstream fileStream(fileName, std::ios::binary);
			if (!fileStream.is_open())
			{
				std::wcout << "Couldn't load file \"" << fileName << "\"" << std::endl;
				char iHateWarnings = getchar();
				exit(69);
			}
			fileStream.seekg(0, std::ios::end);
			size_t fileLen = fileStream.tellg();
			fileStream.seekg(0, std::ios::beg);
			tempFileData.bytes.resize(fileLen);
			fileStream.read((char*)&tempFileData.bytes[0], fileLen);
			fileStream.close();
			return tempFileData;
		}

		// Returns a bitmap from the given image file
		static bitmap GetPixels(std::wstring imagePath)
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
	};
}
