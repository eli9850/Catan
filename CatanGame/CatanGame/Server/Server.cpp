#include "Server.h"
#include "Exceptions/CatanServerExceptions.h"
#include "Exceptions/SocketExceptions.h"

#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>

constexpr std::string_view COMMAND_END_MAGIC = "ABCD1234";

Server::Server(const std::string& port_number) {

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		throw FailedInitializeWinsock("Could not initialize the winsock lib");
	}

	addrinfo* result = nullptr;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(nullptr, port_number.c_str() , &hints, &result) != 0) {
		WSACleanup();
		throw FailedInitializeWinsock("Could not initialize thw winsock lib");
	}

	try {
		m_server_socket = std::make_shared<CatanUtils::WinUtils::Socket>(result);
	} catch (const SocketError&){
		freeaddrinfo(result);
		WSACleanup();
		throw InvalidSocket("Invalid socket");
	}

	if (bind(m_server_socket->get(), result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		freeaddrinfo(result);
		WSACleanup();
		throw BindError("Could not bind the socket");
	}
	freeaddrinfo(result);

	if (listen(m_server_socket->get(), SOMAXCONN) == SOCKET_ERROR) {
		WSACleanup();
		throw ListenError("The socket can not listen to other sockets");
	}
}

void Server::accept_client() {
	CatanUtils::WinUtils::Socket socket = accept(m_server_socket->get(), NULL, NULL);
	if (socket.get() == INVALID_SOCKET) {
		throw AcceptError("Could not accept client");
	}
	m_clients_sockets.push_back(std::move(socket));	
}

std::string Server::recive_data(const uint8_t client) const {
	char recived_data[1024] = { "0" };
	if (recv(m_clients_sockets.at(client).get(), recived_data, 1024, 0) <= 0) {
		throw ReciveError("Could not recive the data with error: " + std::to_string(WSAGetLastError()));
	}
	return recived_data;
}

void Server::send_data(const uint8_t client, const std::string& send_data) const {

	std::string data_with_magic = send_data + COMMAND_END_MAGIC.data();
	if (send(m_clients_sockets.at(client).get(), data_with_magic.c_str(), data_with_magic.size(), 0) != data_with_magic.size()) {
		throw SendError("Could not send the data with error: " + std::to_string(WSAGetLastError()));
	}
}

Server::~Server() {
	try {
		WSACleanup();
	}
	catch (...) {

	}
}
