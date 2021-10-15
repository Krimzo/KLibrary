#pragma once


namespace kl 
{
	class net
	{
	public:
		// Downloads website data(bytes) from the given url
		static bytes GetWebsiteData(std::wstring url)
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

		// Downloads data from the website and saves it in a file
		static void DownloadToFile(std::wstring url, std::wstring fileName)
		{
			file::WriteBytes(GetWebsiteData(url), fileName);
		}

		// Simple TCP client
		class TcpClient
		{
		public:
			// Starts a new tcp client
			void Connect(std::string serverIP, int serverPort, int receiveBufferSize, std::function<std::string(std::string receivedData)> DataProcessing)
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

				// Allocate memory
				char* receivedData = new char[receiveBufferSize];
				if (!receivedData)
				{
					closesocket(sock);
					WSACleanup();
					return;
				}
				memset(receivedData, 0, receiveBufferSize);

				// Communicate with server
				running = true;
				while (true)
				{
					// Edit data
					std::string dataToSend = DataProcessing(receivedData);

					// Check if the client should disconnect
					if (!running)
						break;
					
					// Send data to server
					if (send(sock, dataToSend.c_str(), (int)dataToSend.size() + 1, NULL) == SOCKET_ERROR)
						break;

					// Clear receieve buffer and wait for data to come
					memset(receivedData, 0, receiveBufferSize);
					if (recv(sock, receivedData, receiveBufferSize, NULL) == SOCKET_ERROR)
						break;
				}

				// Cleanup
				delete[] receivedData;
				closesocket(sock);
				WSACleanup();
			}

			// Stops the client
			void Disconnect()
			{
				running = false;
			}

		private:
			bool running = true;
		};
	};
}
