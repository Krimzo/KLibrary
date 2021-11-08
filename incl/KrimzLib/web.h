#pragma once


namespace kl {
	class web {
	public:
		// Downloads website data(bytes) from the given url
		static bytes GetWebsiteData(std::string url, int bufferSize = 16384) {
			// Create browser
			HINTERNET connection = InternetOpenA("CoolBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (!connection) {
				printf("Failed to conenct\n");
				return {};
			}

			// Open url
			HINTERNET openAddress = InternetOpenUrlA(connection, url.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
			if (!openAddress) {
				printf("Failed to open website\n");
				return {};
			}

			// Download data
			bytes finalData;
			bytes dataBuffer(bufferSize);
			DWORD byteReadCount = 0;
			while (InternetReadFile(openAddress, &dataBuffer[0], (DWORD)dataBuffer.size(), &byteReadCount) && byteReadCount) {
				for (DWORD i = 0; i < byteReadCount; i++) {
					finalData.push_back(dataBuffer[i]);
				}
			}

			// Close browser and url
			InternetCloseHandle(openAddress);
			InternetCloseHandle(connection);

			// Return data
			return finalData;
		}

		// Downloads data from the website and saves it in a file
		static void DownloadToFile(std::string url, std::string fileName) {
			file::WriteBytes(fileName, GetWebsiteData(url));
		}
	};
}