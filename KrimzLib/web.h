#pragma once
#include <iostream>
#include <windows.h>
#include <wininet.h>
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Ws2_32.lib")


namespace kl 
{
	class web
	{
	public:
		// Downloads website data(bytes) from the given url
		static bytes GetSiteData(std::wstring url)
		{
			HINTERNET connection = InternetOpen(L"CoolBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (!connection)
			{
				printf("Failed to conenct\n");
				return {};
			}

			HINTERNET openAddress = InternetOpenUrl(connection, url.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
			if (!openAddress)
			{
				printf("Failed to open website\n");
				return {};
			}

			bytes finalData;
			bytes dataBuffer(8192);
			DWORD byteReadCount = 0;
			while (InternetReadFile(openAddress, &dataBuffer[0], (DWORD)dataBuffer.size(), &byteReadCount) && byteReadCount)
			{
				for (DWORD i = 0; i < byteReadCount; i++)
				{
					finalData.push_back(dataBuffer[i]);
				}
			}

			InternetCloseHandle(openAddress);
			InternetCloseHandle(connection);

			return finalData;
		}

		// Downloads data from the internet and saves it in a file
		static void DownloadToFile(std::wstring fileUrl, std::wstring fileName)
		{
			bytes tempBytes = GetSiteData(fileUrl);
			file::WriteBytes(fileName, tempBytes);
		}
	};
}
