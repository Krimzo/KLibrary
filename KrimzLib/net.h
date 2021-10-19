#pragma once


namespace kl {
	class net {
	public:
		// Downloads website data(bytes) from the given url
		static bytes GetWebsiteData(std::wstring url) {
			HINTERNET connection = InternetOpen(L"CoolBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (!connection) {
				printf("Failed to conenct\n");
				return {};
			}

			HINTERNET openAddress = InternetOpenUrl(connection, url.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
			if (!openAddress) {
				printf("Failed to open website\n");
				return {};
			}

			bytes finalData;
			bytes dataBuffer(8192);
			DWORD byteReadCount = 0;
			while (InternetReadFile(openAddress, &dataBuffer[0], (DWORD)dataBuffer.size(), &byteReadCount) && byteReadCount) {
				for (DWORD i = 0; i < byteReadCount; i++)
					finalData.push_back(dataBuffer[i]);
			}

			InternetCloseHandle(openAddress);
			InternetCloseHandle(connection);

			return finalData;
		}

		// Downloads data from the website and saves it in a file
		static void DownloadToFile(std::wstring url, std::wstring fileName) {
			file::WriteBytes(GetWebsiteData(url), fileName);
		}

		// Initialises winsock
		static void InitWinSock() {
			if (!winSockInitialised) {
				WSADATA wsData = {};
				if (WSAStartup(MAKEWORD(2, 2), &wsData))
					printf("Failed to initalise winsock\n");
				winSockInitialised = true;
			}
		}

		// Uninitialises winsock
		static void UninitWinSock() {
			if (winSockInitialised) {
				WSACleanup();
				winSockInitialised = false;
			}
		}

		// Simple TCP server
		class TcpServer {
		public:
			~TcpServer() {
				Destroy();
			}

			// Returns server status
			bool IsCreated() {
				return created;
			}

			// Create a new server
			void Create(int port) {
				if (!created) {
					// Create a socket
					serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
					if (serverSocket == INVALID_SOCKET) {
						printf("Failed to create server socket!\n");
						return;
					}

					// Bind the ip and port to the socket
					sockaddr_in sockHint = {};
					sockHint.sin_family = AF_INET;
					sockHint.sin_port = htons(port);
					sockHint.sin_addr.S_un.S_addr = INADDR_ANY;
					bind(serverSocket, (sockaddr*)&sockHint, sizeof(sockHint));
					listen(serverSocket, SOMAXCONN);
					created = true;
				}
			}

			// Wait for a client to connect
			void WaitForClient() {
				if (!clientConnected) {
					// Wait for a connection
					sockaddr_in client = {};
					int clientSize = sizeof(client);
					clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
					if (clientSocket == INVALID_SOCKET)
						return;

					// Process the connected client data
					char host[NI_MAXHOST];
					char service[NI_MAXSERV];
					memset(host, 0, NI_MAXHOST);
					memset(service, 0, NI_MAXSERV);
					if (!getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, NULL)) {
						clientName = std::string(host);
					}
					else {
						inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
						clientName = std::string(host);
					}
					clientConnected = true;
				}
			}

			// Return the currently connected clients name or empty string if no one is connected
			std::string Client() {
				if (created && clientConnected)
					return clientName;
				return "";
			}

			// Send data back to the client
			void SendData(bytes& data) {
				if (created && clientConnected) {
					if (send(clientSocket, (char*)&data[0], (int)data.size(), NULL) == SOCKET_ERROR)
						clientConnected = false;
				}
			}
			void SendData(bytes&& data, bool echo = false) {
				if (created && clientConnected) {
					if (send(clientSocket, (char*)&data[0], (int)data.size(), NULL) == SOCKET_ERROR)
						clientConnected = false;

					if (echo)
						printf("Data sent!\n");
				}
			}

			// Receive data from the client
			void ReceiveData(bytes& dataBuffer) {
				if (created && clientConnected) {
					memset(&dataBuffer[0], 0, dataBuffer.size());
					if (recv(clientSocket, (char*)&dataBuffer[0], (int)dataBuffer.size(), NULL) == SOCKET_ERROR)
						clientConnected = false;
				}
			}

			// Destroys the server
			void Destroy() {
				closesocket(serverSocket);
				closesocket(clientSocket);
				created = false;
			}

		private:
			SOCKET serverSocket = {};
			SOCKET clientSocket = {};
			bool clientConnected = false;
			bool created = false;
			std::string clientName = "";
		};

		// Simple TCP client
		class TcpClient {
		public:
			~TcpClient() {
				Disconnect();
			}

			// Returns the current state of the client
			bool IsConnected() {
				return connected;
			}

			// Connects the client to server
			void Connect(std::string serverIP, int serverPort) {
				// Disconnect the client if it's already connected
				Disconnect();

				// Create a socket
				clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
				if (clientSocket == INVALID_SOCKET) {
					printf("Failed to create socket!\n");
					return;
				}

				// Connect to the server
				sockaddr_in sockHint = {};
				sockHint.sin_family = AF_INET;
				sockHint.sin_port = htons(serverPort);
				inet_pton(AF_INET, serverIP.c_str(), &sockHint.sin_addr);
				if (connect(clientSocket, (sockaddr*)&sockHint, sizeof(sockHint))) {
					printf("Failed to connect!\n");
					closesocket(clientSocket);
					return;
				}
				connected = true;
			}

			// Send data to server
			void SendData(bytes& data) {
				if (connected) {
					if (send(clientSocket, (char*)&data[0], (int)data.size(), NULL) == SOCKET_ERROR)
						Disconnect();
				}
			}
			void SendData(bytes&& data) {
				if (connected) {
					if (send(clientSocket, (char*)&data[0], (int)data.size(), NULL) == SOCKET_ERROR)
						Disconnect();
				}
			}

			// Receive data from the server
			void ReceiveData(bytes& dataBuffer) {
				if (connected) {
					memset(&dataBuffer[0], 0, dataBuffer.size());
					if (recv(clientSocket, (char*)&dataBuffer[0], (int)dataBuffer.size(), NULL) == SOCKET_ERROR)
						Disconnect();
				}
			}

			// Dsiconnects the client
			void Disconnect() {
				if (connected) {
					closesocket(clientSocket);
					connected = false;
				}
			}

		private:
			SOCKET clientSocket = {};
			bool connected = false;
		};

	private:
		static bool winSockInitialised;
	};
	bool net::winSockInitialised = false;
}
