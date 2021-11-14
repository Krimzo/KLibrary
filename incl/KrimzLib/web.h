#pragma once


namespace kl {
	class web {
	public:
		// Downloads website data(bytes) from the given url
		static kl::bytes GetWebsiteData(std::string url, int bufferSize = 16384) {
			// Create browser
			HINTERNET connection = InternetOpenA("CoolBrowser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
			if (!connection) {
				printf("Failed to conenct\n");
				return {};
			}

			// Open url
			HINTERNET openAddress = InternetOpenUrlA(connection, url.c_str(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
			if (!openAddress) {
				printf("Failed to open website\n");
				return {};
			}

			// Download data
			kl::bytes finalData;
			kl::bytes dataBuffer(bufferSize);
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
			kl::file::WriteBytes(fileName, GetWebsiteData(url));
		}
	};
}
