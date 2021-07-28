#include "CatanClient.h"
#include <iostream>
#include <sstream>
#include <thread>

#include "StringUtils/StringUtils.h"

CatanClient::CatanClient(const std::string& ip, const std::string& port_nubmer) : m_client(ip, port_nubmer) {
	m_number_of_points = 0;
	m_game_is_finished = false;
	m_development_cards.try_emplace(DevelopmentCards::ABUNDANCE_CARD, 0);
	m_development_cards.try_emplace(DevelopmentCards::ROAD_CARD, 0);
	m_development_cards.try_emplace(DevelopmentCards::POINT_CARD, 0);
	m_development_cards.try_emplace(DevelopmentCards::KNIGHT_CARD, 0);

	m_resource_cards.try_emplace(ResourceType::CLAY, 4);
	m_resource_cards.try_emplace(ResourceType::WHEAT, 2);
	m_resource_cards.try_emplace(ResourceType::TREE, 4);
	m_resource_cards.try_emplace(ResourceType::STONE, 0);
	m_resource_cards.try_emplace(ResourceType::SHEEP, 2);
}

void CatanClient::start_game() {
	
	std::string data;
	data = m_client.recive_data();
	auto parsed_data = split(data, "\n");
	std::cout << "Got result: " << parsed_data.at(0) << std::endl;
	std::string board = data.substr(2, data.size() - 2);
	std::cout << "The board is:\n" << data.substr(2, data.size() - 2) << "\n";
	
	m_gui_client.create_catan_board(data.substr(2, data.size() - 2));
	
	std::thread player_func(&CatanClient::handle_player, this);
	std::thread recived_thread(&CatanClient::recive_from_server, this);

	m_gui_client.start_game();

	player_func.join();
	recived_thread.join();
}

void CatanClient::recive_from_server() {
	std::string data;
	while (true) {
		data = "";
		data = m_client.recive_data();
		auto parsed_data = split(data, "\n");
		std::cout << "Got result: " << parsed_data.at(0) << std::endl;
		if (stoi(parsed_data.at(0)) == static_cast<uint8_t>(CommandResult::INFO)) {
			std::cout << "The board is:\n" << data.substr(2, data.size() - 2) << "\n";
			m_gui_client.update_board(data.substr(2, data.size() - 2));
		}
		else if (stoi(parsed_data.at(0)) == static_cast<uint8_t>(CommandResult::YOUR_TURN)) {
			std::cout << "The turn is yours\n";
		}
		else if (stoi(parsed_data.at(0)) == static_cast<uint8_t>(CommandResult::NEW_TURN_INFO)) {
			update_new_turn_info(data);
		}
		else {
			m_command_result.push(data);
		}
	}
}

void CatanClient::handle_player() {
	uint8_t choise = 0;
	while (!m_game_is_finished) {
		std::cout << "enter what you want to do:" << std::endl;
		std::cout << "1. build settlement" << std::endl;
		std::cout << "2. upgrade settelement to city" << std::endl;
		std::cout << "3. build edge" << std::endl;
		std::cout << "4. finish turn" << std::endl;
		std::cout << "5. roll dices" << std::endl;
		std::cout << "6. print resources" << std::endl;
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
		case '5':
			handle_roll_dices();
			continue;
		case '6':
			std::cout << "Tree: " << std::to_string(m_resource_cards.at(ResourceType::TREE)) << std::endl;
			std::cout << "Sheep: " << std::to_string(m_resource_cards.at(ResourceType::SHEEP)) << std::endl;
			std::cout << "Stone: " << std::to_string(m_resource_cards.at(ResourceType::STONE)) << std::endl;
			std::cout << "Wheat: " << std::to_string(m_resource_cards.at(ResourceType::WHEAT)) << std::endl;
			std::cout << "Clay: " << std::to_string(m_resource_cards.at(ResourceType::CLAY)) << std::endl;
			continue;
		default:
			continue;
		}
	}
}

void CatanClient::update_new_turn_info(const std::string& data) {
	auto parsed_data = split(data, "\n");
	auto& dices_str = parsed_data.at(1);
	uint32_t first_dice = atoi(split(dices_str, ",").at(0).c_str());
	uint32_t second_dice = atoi(split(dices_str, ",").at(1).c_str());

	std::cout << "The dice numbers are: " << std::to_string(first_dice) << "," << std::to_string(second_dice) << std::endl;
	m_gui_client.update_dices(first_dice, second_dice);

	if (parsed_data.size() <= 2) {
		return;
	}
	auto& resources_str = parsed_data.at(2);
	for (auto& resource_str : split(resources_str, ";")) {
		auto key = static_cast<ResourceType>(atoi(split(resource_str, ",").at(0).c_str()));
		auto value = stoi(split(resources_str, ",").at(1));
		m_resource_cards.at(key) += value;
	}
	m_gui_client.update_available_resources(m_resource_cards);

}

void CatanClient::handle_build_edge() {
	if (m_resource_cards.at(ResourceType::TREE) < 1 || m_resource_cards.at(ResourceType::CLAY) < 1) {
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
	m_command_result.pop();
	m_resource_cards.at(ResourceType::TREE)--;
	m_resource_cards.at(ResourceType::CLAY)--;
	std::cout << "create the edge" << std::endl;
	m_gui_client.update_available_resources(m_resource_cards);

}

void CatanClient::handle_build_settlement() {
	if (m_resource_cards.at(ResourceType::TREE) < 1 || m_resource_cards.at(ResourceType::CLAY) < 1 ||
		m_resource_cards.at(ResourceType::WHEAT) < 1 || m_resource_cards.at(ResourceType::SHEEP) < 1) {
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
	m_command_result.pop();
	m_resource_cards.at(ResourceType::TREE)--;
	m_resource_cards.at(ResourceType::CLAY)--;
	m_resource_cards.at(ResourceType::SHEEP)--;
	m_resource_cards.at(ResourceType::WHEAT)--;

	std::cout << "create the settlement" << std::endl;
	m_gui_client.update_available_resources(m_resource_cards);
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
	m_command_result.pop();
	std::cout << "your turn is finished" << std::endl;
}

void CatanClient::handle_upgrade_settlement_to_city() {
	if (m_resource_cards.at(ResourceType::STONE) < 3 || m_resource_cards.at(ResourceType::WHEAT) < 2) {
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
		m_command_result.pop();
		std::cout << "could not upgrade the settlement" << std::endl;
		return;
	}
	m_command_result.pop();
	m_resource_cards.at(ResourceType::STONE)--;
	m_resource_cards.at(ResourceType::STONE)--;
	m_resource_cards.at(ResourceType::STONE)--;
	m_resource_cards.at(ResourceType::WHEAT)--;
	m_resource_cards.at(ResourceType::WHEAT)--;

	std::cout << "upgrade the settlement" << std::endl;
	m_gui_client.update_available_resources(m_resource_cards);
}

void CatanClient::handle_roll_dices() {
	std::stringstream command;
	command << std::to_string(static_cast<uint8_t>(CommandType::ROLL_DICES)) << '\n';

	m_client.send_data(command.str());
	while (m_command_result.empty())
	{
		Sleep(100);
	}
	if (m_command_result.front() != std::to_string(static_cast<uint8_t>(CommandResult::SUCCESS))) {
		m_command_result.pop();
		std::cout << "Could not roll the dices" << std::endl;
		return;
	}
	m_command_result.pop();
	std::cout << "roll the dice" << std::endl;
}