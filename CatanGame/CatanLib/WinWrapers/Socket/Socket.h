#pragma once

#include <WinSock2.h>

#pragma comment (lib, "Ws2_32.lib")

namespace CatanUtils {

	namespace WinUtils {

		// RAII class for socket
		class Socket
		{
		public:
			Socket(const SOCKET socket);
			Socket(addrinfo* result);
			~Socket();

			Socket(const Socket&) = delete;
			Socket(Socket&&) noexcept;
			Socket operator=(const Socket&) = delete;
			Socket& operator=(Socket&&) noexcept;

			const SOCKET& get() const;

		private:
			bool m_is_free;
			SOCKET m_socket;
		};
	}
}
