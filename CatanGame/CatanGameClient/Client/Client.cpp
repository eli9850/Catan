#include "Client.h"

#include "Exceptions/ClientExceptions.h"

#include <Windows.h>
#include <WS2tcpip.h>

#include "Exceptions/SocketExceptions.h"

Client::Client(const std::string& ip, const std::string& port_number)
{
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		throw FailedInitializeWinsock("Could not initialize thw winsock lib");
	}

	addrinfo* result = nullptr;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(ip.c_str(), port_number.c_str(), &hints, &result) != 0)
	{
		WSACleanup();
		throw FailedInitializeWinsock("Could not initialize thw winsock lib");
	}

	try
	{
		m_socket = std::make_shared<CatanUtils::WinUtils::Socket>(result);
	}
	catch (const SocketError&)
	{
		freeaddrinfo(result);
		WSACleanup();
		throw InvalidSocket("Invalid socket");
	}

	if (connect(m_socket->get(), result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		throw ConnectError("Could not connect to the server");
	}

	freeaddrinfo(result);

	if (m_socket->get() == INVALID_SOCKET)
	{
		WSACleanup();
		throw ConnectError("Could not connect to the server");
	}
}

void Client::send_data(const std::string& data) const
{
	if (send(m_socket->get(), data.c_str(), data.size(), 0) != data.size())
	{
		throw SendError("Could not send the data with error: " + std::to_string(WSAGetLastError()));
	}
}

std::string Client::receive_data() const
{
	char received_data[1024] = { "0" };
	if (recv(m_socket->get(), received_data, 1024, 0) <= 0)
	{
		throw ReciveError("Could not receive the data with error: " + std::to_string(WSAGetLastError()));
	}
	return received_data;
}

Client::~Client()
{
	try
	{
		WSACleanup();
	}
	catch (...)
	{
	}
}
