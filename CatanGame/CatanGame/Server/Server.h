#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include "WinWrapers/Socket/Socket.h"

class Server final
{
public:
	explicit Server(const std::string& port_number);

	void accept_client();
	std::string receive_data(const uint32_t client) const;
	void send_data(const uint32_t client, const std::string& send_data) const;

	virtual ~Server();

private:
	std::shared_ptr<CatanUtils::WinUtils::Socket> m_server_socket;
	std::vector<CatanUtils::WinUtils::Socket> m_clients_sockets;
};
