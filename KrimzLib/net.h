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

		// Simple TCP server
		class TcpServer
		{
		public:
			// Vars
			std::function<std::string(std::string receivedData)> DataProcessing = [](std::string receivedData) { return receivedData; };

			// Starts the server
			void Start(std::string serverIP, int serverPort, int receiveBufferSize)
			{
				// Init winsock
				WSADATA wsData = {};
				if (WSAStartup(MAKEWORD(2, 2), &wsData))
				{
					printf("Failed to init winsock!\n");
					return;
				}

				// Create a socket
				SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
				if (serverSocket == INVALID_SOCKET)
				{
					std::cout << "Failed to create server socket!, Error = " << WSAGetLastError() << std::endl;
					WSACleanup();
					return;
				}

				// Bind the ip and port to the socket
				sockaddr_in sockHint = {};
				sockHint.sin_family = AF_INET;
				sockHint.sin_port = htons(serverPort);
				sockHint.sin_addr.S_un.S_addr = INADDR_ANY;
				bind(serverSocket, (sockaddr*)&sockHint, sizeof(sockHint));

				// Prepare for listening
				listen(serverSocket, SOMAXCONN);

				// Memory allocation
				running = true;
				char* receiveBuffer = new char[receiveBufferSize];
				if (!receiveBuffer)
				{
					closesocket(serverSocket);
					WSACleanup();
				}

				// Communication
				SOCKET clientSocket = {};
				while (true)
				{
					if (!clientConnected)
					{
						// Wait for a connection
						sockaddr_in client = {};
						int clientSize = sizeof(client);
						std::cout << "Waiting for clients on " << serverIP << ":" << serverPort << std::endl;
						clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
						if (clientSocket == INVALID_SOCKET)
						{
							std::cout << "Failed to accept! Error = " << WSAGetLastError() << std::endl;
							closesocket(serverSocket);
							WSACleanup();
							return;
						}
						clientConnected = true;

						// Process the connected client data
						char host[NI_MAXHOST];
						char service[NI_MAXSERV];
						memset(host, 0, NI_MAXHOST);
						memset(service, 0, NI_MAXSERV);
						if (!getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, NULL))
						{
							std::cout << host << " connected on port " << service << std::endl;
						}
						else
						{
							inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
							std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
						}
					}

					// Clear buffer and wait for data
					memset(receiveBuffer, 0, receiveBufferSize);
					if (recv(clientSocket, receiveBuffer, receiveBufferSize, NULL) == SOCKET_ERROR)
					{
						std::cout << "Client disconnected!\n";
						clientConnected = false;
						continue;
					}

					// Process data
					std::string dataToSend = DataProcessing(receiveBuffer);

					// Check if the server should stop running
					if (!running)
						break;

					// Send data back to the client
					if (send(clientSocket, dataToSend.c_str(), (int)dataToSend.size() + 1, NULL) == SOCKET_ERROR)
					{
						std::cout << "Client disconnected!\n";
						clientConnected = false;
						continue;
					}
				}

				// Cleanup
				delete[] receiveBuffer;
				closesocket(serverSocket);
				closesocket(clientSocket);
				WSACleanup();
			}

			// Stos the server
			void Stop()
			{
				running = false;
			}

		private:
			bool clientConnected = false;
			bool running = true;
		};

		// Simple TCP client
		class TcpClient
		{
		public:
			// Vars
			std::function<std::string(std::string receivedData)> DataProcessing = [](std::string receivedData) { return receivedData; };

			// Starts a new tcp client
			void Connect(std::string serverIP, int serverPort, int receiveBufferSize)
			{
				// Init winsock
				WSADATA wsData = {};
				if (WSAStartup(MAKEWORD(2, 2), &wsData))
				{
					printf("Failed to init winsock!\n");
					return;
				}

				// Create a socket
				SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
				if (clientSocket == INVALID_SOCKET)
				{
					std::cout << "Failed to create socket!, Error = " << WSAGetLastError() << std::endl;
					WSACleanup();
					return;
				}
	
				// Connect to the server
				sockaddr_in sockHint = {};
				sockHint.sin_family = AF_INET;
				sockHint.sin_port = htons(serverPort);
				inet_pton(AF_INET, serverIP.c_str(), &sockHint.sin_addr);
				std::cout << "Trying to connect to " << serverIP << ":" << serverPort << std::endl;
				if (connect(clientSocket, (sockaddr*)&sockHint, sizeof(sockHint)))
				{
					std::cout << "Failed to connect! Error = " << WSAGetLastError() << std::endl;
					closesocket(clientSocket);
					WSACleanup();
					return;
				}
				printf("Connected!\n");

				// Allocate memory
				char* receivedData = new char[receiveBufferSize];
				if (!receivedData)
				{
					closesocket(clientSocket);
					WSACleanup();
					return;
				}
				memset(receivedData, 0, receiveBufferSize);

				// Communicate with server
				connected = true;
				while (true)
				{
					// Edit data
					std::string dataToSend = DataProcessing(receivedData);

					// Check if the client should disconnect
					if (!connected)
						break;
					
					// Send data to server
					if (send(clientSocket, dataToSend.c_str(), (int)dataToSend.size() + 1, NULL) == SOCKET_ERROR)
						break;

					// Clear receieve buffer and wait for data to come
					memset(receivedData, 0, receiveBufferSize);
					if (recv(clientSocket, receivedData, receiveBufferSize, NULL) == SOCKET_ERROR)
						break;
				}

				// Cleanup
				delete[] receivedData;
				closesocket(clientSocket);
				WSACleanup();
			}

			// Stops the client
			void Disconnect()
			{
				connected = false;
			}

		private:
			bool connected = true;
		};
	};
}
