#pragma once


namespace kl {
	class tcp {
	public:
		// Simple TCP server
		class server {
		public:
			~server() {
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
					serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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
					if (clientSocket == INVALID_SOCKET) {
						return;
					}

					// Process the connected client data
					char host[NI_MAXHOST];
					char service[NI_MAXSERV];
					memset(host, 0, NI_MAXHOST);
					memset(service, 0, NI_MAXSERV);
					if (!getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0)) {
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
				if (created && clientConnected) {
					return clientName;
				}
				return "";
			}

			// Send data back to the client
			void SendData(kl::bytes& data) {
				if (created && clientConnected) {
					if (send(clientSocket, (char*)&data[0], (int)data.size(), 0) == SOCKET_ERROR) {
						clientConnected = false;
					}
				}
			}
			void SendData(kl::bytes&& data) {
				SendData(data);
			}

			// Receive data from the client
			void ReceiveData(kl::bytes& dataBuffer) {
				if (created && clientConnected) {
					memset(&dataBuffer[0], 0, dataBuffer.size());
					if (recv(clientSocket, (char*)&dataBuffer[0], (int)dataBuffer.size(), 0) == SOCKET_ERROR) {
						clientConnected = false;
					}
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
		class client {
		public:
			~client() {
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
				clientSocket = socket(AF_INET, SOCK_STREAM, 0);
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
			void SendData(kl::bytes& data) {
				if (connected) {
					if (send(clientSocket, (char*)&data[0], (int)data.size(), 0) == SOCKET_ERROR) {
						Disconnect();
					}
				}
			}
			void SendData(kl::bytes&& data) {
				SendData(data);
			}

			// Receive data from the server
			void ReceiveData(kl::bytes& dataBuffer) {
				if (connected) {
					memset(&dataBuffer[0], 0, dataBuffer.size());
					if (recv(clientSocket, (char*)&dataBuffer[0], (int)dataBuffer.size(), 0) == SOCKET_ERROR) {
						Disconnect();
					}
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

	protected:
		// Initialises winsock
		static void InitWinSock() {
			WSADATA wsData = {};
			if (WSAStartup(MAKEWORD(2, 2), &wsData)) {
				printf("Failed to initalise winsock\n");
				kl::console::WaitFor(' ', true);
				exit(69);
			}
		}

		// Uninitialises winsock
		static void UninitWinSock() {
			WSACleanup();
		}
	};
}