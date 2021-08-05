#include "Server.h"
#include "Exceptions/CatanServerExceptions.h"
#include "Exceptions/SocketExceptions.h"

#include <Windows.h>
#include <WS2tcpip.h>
#include <iostream>

#include "Catan/CatanConsts.h"

Server::Server(const std::string& port_number)
{
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		throw FailedInitializeWinsock("Could not initialize the winsock lib");
	}

	addrinfo* result = nullptr;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(nullptr, port_number.c_str(), &hints, &result) != 0)
	{
		WSACleanup();
		throw FailedInitializeWinsock("Could not initialize thw winsock lib");
	}

	try
	{
		m_server_socket = std::make_shared<CatanUtils::WinUtils::Socket>(result);
	}
	catch (const SocketError&)
	{
		freeaddrinfo(result);
		WSACleanup();
		throw InvalidSocket("Invalid socket");
	}

	if (bind(m_server_socket->get(), result->ai_addr, static_cast<int>(result->ai_addrlen)) ==
		SOCKET_ERROR)
	{
		freeaddrinfo(result);
		WSACleanup();
		throw BindError("Could not bind the socket");
	}
	freeaddrinfo(result);

	if (listen(m_server_socket->get(), SOMAXCONN) == SOCKET_ERROR)
	{
		WSACleanup();
		throw ListenError("The socket can not listen to other sockets");
	}
}

void Server::accept_client()
{
	CatanUtils::WinUtils::Socket socket(accept(m_server_socket->get(), nullptr, nullptr));
	if (socket.get() == INVALID_SOCKET)
	{
		throw AcceptError("Could not accept client");
	}
	m_clients_sockets.push_back(std::move(socket));
}

std::string Server::receive_data(const uint32_t client) const
{
	char received_data[1024] = { "0" };
	if (recv(m_clients_sockets.at(client).get(), received_data, 1024, 0) <= 0)
	{
		throw ReciveError("Could not receive the data with error: " + std::to_string(WSAGetLastError()));
	}
	return received_data;
}

void Server::send_data(const uint32_t client, const std::string& send_data) const
{
	const auto data_with_magic = send_data + CatanUtils::Consts::COMMAND_END_MAGIC.data();
	if (send(m_clients_sockets.at(client).get(), data_with_magic.c_str(), data_with_magic.size(), 0) !=
		data_with_magic.size())
	{
		throw SendError("Could not send the data with error: " + std::to_string(WSAGetLastError()));
	}
}

Server::~Server()
{
	try
	{
		WSACleanup();
	}
	catch (...)
	{
	}
}
