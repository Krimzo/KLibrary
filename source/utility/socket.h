#pragma once

#include <iostream>
#include <windows.h>

#include "math/math.h"


namespace kl {
	class socket {
	private:
		static WSADATA m_WSAData;
		static bool m_WSAInit;
		sockaddr_in m_Address = {};
		SOCKET m_Socket = NULL;

	public:
		static void initWSA();
		static void uninitWSA();

		socket();
		socket(const SOCKET& sock);
		~socket();

		operator SOCKET();

		void close();

		void port(uint port);
		void address(const std::string& addrs);

		uint port() const;
		std::string address() const;

		void bind();
		void listen(int queueSize);
		kl::socket accept();
		void connect();

		int send(const void* data, uint byteSize);
		int receive(void* buff, uint byteSize);
	};
}
