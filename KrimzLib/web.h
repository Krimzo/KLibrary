#pragma once
#include <iostream>
#include <ws2tcpip.h>
#pragma comment (lib, "ws2_32.lib")


namespace kl 
{
	class web
	{
	public:

		void temp()
		{
			// Init winsock
			WSADATA wsData;
			if (WSAStartup(MAKEWORD(2, 2), &wsData))
				exit(-1);

			// Creating a socket
			SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, NULL);
			if (listeningSocket == INVALID_SOCKET)
				exit(-1);

			// Binding ip and port to a socket
			sockaddr_in listeningHint;
			listeningHint.sin_family = AF_INET;
			listeningHint.sin_port = htons(80);
			listeningHint.sin_addr.S_un.S_addr = INADDR_ANY;
			bind(listeningSocket, (sockaddr*)&listeningHint, sizeof(listeningHint));

			// Tell ws the socket is for listening
			listen(listeningSocket, SOMAXCONN);

			// Wait for a connection
			sockaddr_in client;
			int clientSize = sizeof(client);
			printf("Waiting for a connection.\n");
			SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);
			if (clientSocket == INVALID_SOCKET)
				exit(-1);

			// Getting client information
			const int infoBufferSize = 1000;
			char hostBuffer[infoBufferSize];
			char serviceBuffer[infoBufferSize];
			memset(hostBuffer, 0, infoBufferSize);
			memset(serviceBuffer, 0, infoBufferSize);

			if (!getnameinfo((sockaddr*)&client, sizeof(client), hostBuffer, infoBufferSize, serviceBuffer, infoBufferSize, NULL))
			{
				std::cout << "Host connected on port " << serviceBuffer << std::endl;
			}
			else
			{
				inet_ntop(AF_INET, &client.sin_addr, hostBuffer, infoBufferSize);
				std::cout << "Host connected on port " << ntohs(client.sin_port) << std::endl;
			}

			// Closing the listening socket
			closesocket(listeningSocket);

			// Data buffer and waiting for data
			const int dataBufferSize = 4 * 1024;
			char dataBuffer[dataBufferSize];
			while (true)
			{
				// Reset buffer
				memset(dataBuffer, 0, dataBufferSize);

				// Wait for client to send data
				int byteSize = recv(clientSocket, dataBuffer, dataBufferSize, NULL);
				if (byteSize == SOCKET_ERROR)
					exit(-1);
				else if (byteSize == 0)
					printf("Client disconnected!\n");
				break;

				/* Process data */
				// Printing data to the console
				std::cout << dataBuffer << std::endl;

				// Send data back
				send(clientSocket, dataBuffer, byteSize + 1, NULL);
			}

			// Closes client socket
			closesocket(clientSocket);

			// Uninit winsock
			WSACleanup();
		}

	};
}
