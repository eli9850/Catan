#include "Socket.h"

#include "Exceptions/SocketExceptions.h"

Socket::Socket(addrinfo* result) {
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		throw FailedInitializeSocket("failed to initialize socket");
	}
	m_is_free = false;
}

Socket::Socket(const SOCKET socket) {
	m_socket = socket;
}

const SOCKET& Socket::get() const {
	return m_socket;
}

Socket::Socket(Socket&& socket) noexcept {
	m_socket = socket.m_socket;
	m_is_free = false;
	socket.m_socket = NULL;
	socket.m_is_free = true;
	
}

Socket& Socket::operator=(Socket&& socket) noexcept {
	m_socket = socket.m_socket;
	m_is_free = false;
	socket.m_socket = NULL;
	socket.m_is_free = true;
	return *this;

}

Socket::~Socket() {
	if (!m_is_free) {
		try {
			closesocket(m_socket);
		}
		catch (...) {

		}
	}
}