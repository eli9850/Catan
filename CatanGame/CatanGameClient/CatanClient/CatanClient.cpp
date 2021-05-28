#include "CatanClient.h"
#include <iostream>
#include <sstream>
#include <thread>
#include "StringUtils/StringUtils.h"

CatanClient::CatanClient(const std::string& ip, const std::string& port_nubmer) : m_client(ip, port_nubmer) {
	m_number_of_points = 0;
	m_game_is_finished = false;
	m_development_cards[DevelopmentCards::ABUNDANCE_CARD] = 0;
	m_development_cards[DevelopmentCards::ROAD_CARD] = 0;
	m_development_cards[DevelopmentCards::POINT_CARD] = 0;
	m_development_cards[DevelopmentCards::KNIGHT_CARD] = 0;

	m_resource_cards[ResourceType::CLAY] = 10;
	m_resource_cards[ResourceType::WHEAT] = 10;
	m_resource_cards[ResourceType::TREE] = 10;
	m_resource_cards[ResourceType::STONE] = 10;
	m_resource_cards[ResourceType::SHEEP] = 10;
}

void CatanClient::start_game() {

	std::thread recived_thread(&CatanClient::recive_from_server, this);
	uint8_t choise = 0;
	while (!m_game_is_finished) {
		std::cout << "enter what you want to do:" << std::endl;
		std::cout << "1. build settlement" << std::endl;
		std::cout << "2. upgrade settelement to city" << std::endl;
		std::cout << "3. build edge" << std::endl;
		std::cout << "4. finish turn" << std::endl;
		std::cin >> choise;
		switch (choise)
		{
		case '1':
			handle_build_settlement();
			continue;
		case '2':
			handle_upgrade_settlement_to_city();
			continue;
		case '3':
			handle_build_edge();
			continue;
		case '4':
			handle_finish_turn();
			continue;
		default:
			continue;
		}
	}
	recived_thread.join();
}

void CatanClient::recive_from_server() {
	std::string data;
	while (true) {
		data = m_client.recive_data();
		auto parsed_data = split(data, "\n");
		std::cout << "Got result: " << parsed_data[0] << std::endl;
		if (stoi(parsed_data[0]) == static_cast<uint8_t>(CommandResult::INFO)) {
			std::cout << "The board is:\n" << data.substr(2, data.size() - 2) << "\n";
		}
		else if (stoi(parsed_data[0]) == static_cast<uint8_t>(CommandResult::YOUR_TURN)) {
			std::cout << "The turn is yours\n";
		}
		else {
			m_command_result.push(data);
		}
	}
}

void CatanClient::handle_build_edge() {
	if (m_resource_cards[ResourceType::TREE] < 1 || m_resource_cards[ResourceType::CLAY] < 1) {
		std::cout << "not enough source" << std::endl;
		return;
	}
	std::string row_number;
	std::string col_number;
	std::cout << "enter row number: ";
	std::cin >> row_number;
	std::cout << "enter col number: ";
	std::cin >> col_number;
	
	std::stringstream command;
	command << std::to_string(static_cast<uint8_t>(CommandType::EDGE)) << '\n';
	command << row_number << "," << col_number;

	m_client.send_data(command.str());
	while (m_command_result.empty())
	{
		Sleep(100);
	}
	if (m_command_result.front() != std::to_string(static_cast<uint8_t>(CommandResult::SUCCESS))) {
		m_command_result.pop();
		std::cout << "could not create the edge" << std::endl;
		return;
	}

	m_resource_cards[ResourceType::TREE]--;
	m_resource_cards[ResourceType::CLAY]--;
	std::cout << "create the edge" << std::endl;

}

void CatanClient::handle_build_settlement() {
	if (m_resource_cards[ResourceType::TREE] < 1 || m_resource_cards[ResourceType::CLAY] < 1 ||
		m_resource_cards[ResourceType::WHEAT] < 1 || m_resource_cards[ResourceType::SHEEP] < 1) {
		std::cout << "not enough source" << std::endl;
		return;
	}
	std::string row_number;
	std::string col_number;
	std::cout << "enter row number: ";
	std::cin >> row_number;
	std::cout << "enter col number: ";
	std::cin >> col_number;

	std::stringstream command;
	command << std::to_string(static_cast<uint8_t>(CommandType::SETTLEMENT)) << '\n';
	command << row_number << "," << col_number;

	m_client.send_data(command.str());
	while (m_command_result.empty())
	{
		Sleep(100);
	}
	if (m_command_result.front() != std::to_string(static_cast<uint8_t>(CommandResult::SUCCESS))) {
		m_command_result.pop();
		std::cout << "could not create the settlement" << std::endl;
		return;
	}

	m_resource_cards[ResourceType::TREE]--;
	m_resource_cards[ResourceType::CLAY]--;
	m_resource_cards[ResourceType::SHEEP]--;
	m_resource_cards[ResourceType::WHEAT]--;

	std::cout << "create the settlement" << std::endl;
}

void CatanClient::handle_finish_turn() {
	
	std::stringstream command;
	command << std::to_string(static_cast<uint8_t>(CommandType::FINISH_TURN)) << '\n';

	m_client.send_data(command.str());
	while (m_command_result.empty())
	{
		Sleep(100);
	}
	if (m_command_result.front() != std::to_string(static_cast<uint8_t>(CommandResult::SUCCESS))) {
		m_command_result.pop();
		std::cout << "could not finish turn" << std::endl;
		return;
	}

	std::cout << "your turn is finished" << std::endl;
}

void CatanClient::handle_upgrade_settlement_to_city() {
	if (m_resource_cards[ResourceType::STONE] < 3 || m_resource_cards[ResourceType::WHEAT] < 2) {
		std::cout << "not enough source" << std::endl;
		return;
	}
	std::string row_number;
	std::string col_number;
	std::cout << "enter row number: ";
	std::cin >> row_number;
	std::cout << "enter col number: ";
	std::cin >> col_number;

	std::stringstream command;
	command << std::to_string(static_cast<uint8_t>(CommandType::CITY)) << '\n';
	command << row_number << "," << col_number;

	m_client.send_data(command.str());
	while (m_command_result.empty())
	{
		Sleep(100);
	}
	if (m_command_result.front() != std::to_string(static_cast<uint8_t>(CommandResult::SUCCESS))) {
		std::cout << "could not upgrade the settlement" << std::endl;
		return;
	}

	m_resource_cards[ResourceType::STONE]--;
	m_resource_cards[ResourceType::STONE]--;
	m_resource_cards[ResourceType::STONE]--;
	m_resource_cards[ResourceType::WHEAT]--;
	m_resource_cards[ResourceType::WHEAT]--;

	std::cout << "upgrade the settlement" << std::endl;
}