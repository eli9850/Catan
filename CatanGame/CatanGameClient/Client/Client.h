#pragma once

#include <memory>
#include <string>

#include "WinWrapers/Socket/Socket.h"

#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class Client
{
public:
	Client(const std::string& ip, const std::string& port_number);

	std::string receive_data() const;
	void send_data(const std::string& data) const;

	virtual ~Client();

private:
	std::shared_ptr<CatanUtils::WinUtils::Socket> m_socket;
};
