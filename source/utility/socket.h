#pragma once

#include <iostream>
#include <windows.h>

#include "math/math.h"


namespace kl {
	class socket {
	public:
		static void init();
		static void uninit();
		static const std::string self;

	private:
		sockaddr_in m_Address = {};
		uint64 m_Socket = {};

	public:
		socket();
		socket(const std::string& address, uint port);
		socket(uint64 socket);
		~socket();

		operator uint64();

		void close();

		std::string address() const;
		void address(const std::string& addrs);
		uint port() const;
		void port(uint port);

		void listen(uint queueSize);
		kl::socket accept();
		void connect();

		int send(const void* data, uint byteSize) const;
		int receive(void* buff, uint byteSize) const;
		template<typename T> int send(const T& obj) const {
			return send(&obj, sizeof(T));
		}
		template<typename T> int receive(T& obj) const {
			return receive(&obj, sizeof(T));
		}
	};
}
