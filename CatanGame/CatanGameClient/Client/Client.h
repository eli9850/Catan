#pragma once

#include <memory>
#include <string>

#include "Socket.h"

#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class Client
{
public:
	Client(const std::string& ip, const std::string& port_number);

	std::string recive_data() const;
	void send_data(const std::string& send_data) const;

	virtual ~Client();

private:
	std::shared_ptr<Socket> m_socket;
};
