#pragma once


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
			file::WriteBytes(GetSiteData(fileUrl), fileName);
		}

		// Simple TCP client
		// Keeps sending and receiving data until sent/received data is 0 bytes
		static void TcpClient(std::string serverIP, int serverPort, int receiveBufferSize, std::function<void(std::string& dataToSend, std::string receivedData)> DataEdit)
		{
			// Init winsock
			WSADATA wsData = {};
			if (WSAStartup(MAKEWORD(2, 2), &wsData))
			{
				printf("Couldn't init winsock!\n");
				return;
			}

			// Create a socket
			SOCKET sock = socket(AF_INET, SOCK_STREAM, NULL);
			if (sock == INVALID_SOCKET)
			{
				printf("Couldn't create socket!, Error = %d\n", WSAGetLastError());
				WSACleanup();
				return;
			}

			// Connect to the server
			sockaddr_in sockHint = {};
			sockHint.sin_family = AF_INET;
			sockHint.sin_port = htons(serverPort);
			inet_pton(AF_INET, serverIP.c_str(), &sockHint.sin_addr);
			if (connect(sock, (sockaddr*)&sockHint, sizeof(sockHint)))
			{
				printf("Couldn't connect to the server! Error = %d\n", WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				return;
			}

			// Alloate memory
			std::string dataToSend;
			char* receieveBuffer = new char[receiveBufferSize];
			if (!receieveBuffer)
				return;
			memset(receieveBuffer, 0, receiveBufferSize);

			// Keep communicating
			while (true)
			{
				// Edit data
				DataEdit(dataToSend, receieveBuffer);

				// Check if data is not 0 otherwise quit
				if (dataToSend.size())
				{
					// Send data and clear send buffer
					send(sock, dataToSend.c_str(), (int)dataToSend.size() + 1, NULL);
					dataToSend = "";

					// Clear receieve buffer and wait for data to come
					memset(receieveBuffer, 0, receiveBufferSize);
					recv(sock, receieveBuffer, receiveBufferSize, NULL);

					// Check if received data is not 0 otherwise quit
					if (!receieveBuffer[0])
						break;
				}
				else
					break;
			}

			// Cleanup
			delete[] receieveBuffer;
			closesocket(sock);
			WSACleanup();
		}
	};
}
