#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include "Socket.h"

constexpr uint32_t CATAN_PORT = 12345;

class Server
{
public:
	explicit Server(const std::string& port_number);

	void accept_client();

	std::string recive_data(const uint8_t client) const;

	void send_data(const uint8_t client, const std::string& send_data) const;

	virtual ~Server();

private:
	std::shared_ptr<Socket> m_server_socket;
	std::vector<Socket> m_clients_sockets;
};

