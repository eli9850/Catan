#pragma once

#include <stdint.h>
#include <winsock2.h>
#include <vector>
#include <memory>
#include <string>

#include "Socket.h"

constexpr uint32_t CATAN_PORT = 12345;

class CatanServer
{
public:
	explicit CatanServer(const uint16_t port_number);

	void accept_client();

	std::string recive_data(const uint8_t player_number) const;

	void send_data(const uint8_t player_number, const std::string& send_data) const;

	virtual ~CatanServer();

private:
	std::shared_ptr<Socket> m_server_socket;
	std::vector<Socket> m_clients_sockets;
};

