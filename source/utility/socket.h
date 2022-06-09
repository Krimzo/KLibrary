#pragma once

#include <iostream>
#include <windows.h>


namespace kl {
	class socket {
	private:
		static WSADATA wsaData;
		static bool wsaInited;
		sockaddr_in addr = {};
		SOCKET sock = NULL;

	public:
		// WSA setup
		static void initWSA();
		static void uninitWSA();

		socket();
		socket(SOCKET sock);
		~socket();

		operator SOCKET();

		// Closes the socket
		void close();

		// Info setters
		void setPort(uint16_t port);
		void setAddr(const std::string& addrs);

		// Binds the socket
		void bind();

		// Sets socket mode to listen
		void listen(int queueSize);

		// Accepts new connections
		kl::socket accept();

		// Connects to a socket
		void connect();

		// Sends data to socket
		int send(const void* data, int byteSize);

		// Receives data from socket
		int recieve(void* buff, int byteSize);
	};
}
