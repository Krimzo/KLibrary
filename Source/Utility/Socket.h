#pragma once

#include <iostream>
#include <windows.h>

#include "Math/Math.h"


namespace kl {
	class Socket {
		sockaddr_in m_Address = {};
		uint64 m_Socket = {};

	public:
		static void Init();
		static void Uninit();
		static const String Self;

		Socket();
		Socket(const String& address, uint port);
		Socket(uint64 socket);
		~Socket();

		operator uint64();

		void close();

		String getAddress() const;
		void setAddress(const String& addrs);

		uint getPort() const;
		void setPort(uint port);

		void listen(uint queueSize);
		Socket accept();
		void connect();

		int send(const void* data, uint byteSize) const;
		int receive(void* buff, uint byteSize) const;

		template<typename T>
		int send(const T& obj) const {
			return send(&obj, sizeof(T));
		}

		template<typename T>
		int receive(T& obj) const {
			return receive(&obj, sizeof(T));
		}
	};
}
