#include "CatanClient.h"
#include <iostream>
#include <sstream>
#include <thread>

#include "Utils/StringUtils.h"

CatanClient::CatanClient(const std::string& ip, const std::string& port_number) :
	m_client(ip, port_number), m_game_is_finished(false)
{
	const auto connect_info = m_client.receive_data();
	auto parsed_data = CatanUtils::StringUtils::split(connect_info, "\n");
	if (static_cast<CatanUtils::ServerInfo>(std::stoi(parsed_data.at(0))) !=
		CatanUtils::ServerInfo::CONNECT_INFO)
	{
		std::stringstream command;
		command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::EXIT_GAME));
		m_client.send_data(command.str());
	}
	m_player = std::make_unique<CatanUtils::Player>(CatanUtils::PlayerType::BLUE);
}

void CatanClient::start_game()
{
	std::thread get_commands_thread(&CatanClient::get_commands_from_server, this);

	const auto data = m_commands.pop_and_front();
	auto parsed_data = CatanUtils::StringUtils::split(data, "\n");

	if (static_cast<CatanUtils::ServerInfo>(std::stoi(parsed_data.at(0))) !=
		CatanUtils::ServerInfo::BOARD_INFO)
	{
		std::stringstream command;
		command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::EXIT_GAME));
		m_client.send_data(command.str());
	}

	const auto board = data.substr(2, data.size() - 2);
	m_gui_client.create_catan_board(board);

	std::thread handle_player_thread(&CatanClient::handle_player, this);
	std::thread handle_server_commands_thread(&CatanClient::handle_server_commands, this);

	m_gui_client.start_game();

	get_commands_thread.join();
	handle_player_thread.join();
	handle_server_commands_thread.join();
}

void CatanClient::get_commands_from_server()
{
	while (true)
	{
		const auto data = m_client.receive_data();
		for (auto& command : CatanUtils::StringUtils::split(
			     data, CatanUtils::Consts::COMMAND_END_MAGIC.data()))
		{
			auto parsed_command = CatanUtils::StringUtils::split(command, "\n");
			std::cout << "Got result: " << parsed_command.at(0) << std::endl;

			switch (static_cast<CatanUtils::ServerInfo>(std::stoi(parsed_command.at(0))))
			{
			case CatanUtils::ServerInfo::CREATE_EDGE_SUCCEEDED:
			case CatanUtils::ServerInfo::CREATE_SETTLEMENT_SUCCEEDED:
			case CatanUtils::ServerInfo::CREATE_CITY_SUCCEEDED:
			case CatanUtils::ServerInfo::FINISH_TURN_SUCCEEDED:
			case CatanUtils::ServerInfo::ROB_RESOURCES_SUCCEEDED:
			case CatanUtils::ServerInfo::MOVE_KNIGHT_SUCCEEDED:
			case CatanUtils::ServerInfo::ROLL_DICES_SUCCEEDED:
			case CatanUtils::ServerInfo::ROB_PLAYER_SUCCEEDED:
			case CatanUtils::ServerInfo::NOT_YOUR_TURN:
			case CatanUtils::ServerInfo::ONLY_SETTLEMENT:
			case CatanUtils::ServerInfo::ONLY_EDGE:
			case CatanUtils::ServerInfo::WRONG_NUMBER_OF_RESOURCES_TO_ROB:
			case CatanUtils::ServerInfo::INVALID_EDGE_PLACE:
			case CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE:
			case CatanUtils::ServerInfo::NOT_ENOUGH_RESOURCES:
			case CatanUtils::ServerInfo::ONLY_ROBBER:
			case CatanUtils::ServerInfo::KNIGHT:
			case CatanUtils::ServerInfo::ONLY_KNIGHT:
			case CatanUtils::ServerInfo::INVALID_RESOURCE_PLACE:
			case CatanUtils::ServerInfo::INVALID_PLAYER_TO_ROB:
			case CatanUtils::ServerInfo::KNIGHT_ROB:
			case CatanUtils::ServerInfo::ONLY_PLAYER_TO_ROB:
			case CatanUtils::ServerInfo::BUY_DEVELOPMENT_CARD_SUCCEEDED:
			case CatanUtils::ServerInfo::NO_MORE_DEVELOPMENT_CARDS:
			case CatanUtils::ServerInfo::NOT_ENOUGH_DEVELOPMENT_CARDS:
			case CatanUtils::ServerInfo::ROAD_BUILDING_CARD_SUCCEEDED:
			case CatanUtils::ServerInfo::ABUNDANCE_CARD_SUCCEEDED:
			case CatanUtils::ServerInfo::INVALID_RESOURCE_TYPE:
			case CatanUtils::ServerInfo::MONOPOLY_CARD_SUCCEEDED:
				m_command_result.push(command);
				break;
			default:
				m_commands.push(command);
			}
		}
	}
}

void CatanClient::handle_server_commands()
{
	while (true)
	{
		const auto data = m_commands.pop_and_front();
		auto parsed_data = CatanUtils::StringUtils::split(data, "\n");
		switch (static_cast<CatanUtils::ServerInfo>(std::stoi(parsed_data.at(0))))
		{
		case CatanUtils::ServerInfo::BOARD_INFO:
			m_gui_client.update_board(data.substr(2, data.size() - 2));
			break;
		case CatanUtils::ServerInfo::YOUR_TURN:
			std::cout << "The turn is yours\n";
			m_is_my_turn.set_event();
			break;
		case CatanUtils::ServerInfo::NEW_RESOURCES:
			update_new_resources_info(data);
			break;
		case CatanUtils::ServerInfo::NEW_DEVELOPMENT_CARDS:
			update_new_development_cards_info(data);
			break;
		case CatanUtils::ServerInfo::DICES_NUMBERS:
			update_dices(parsed_data.at(1));
			break;
		case CatanUtils::ServerInfo::ROBBER:
			if (WaitForSingleObject(m_is_my_turn.get(), 0) != WAIT_TIMEOUT)
			{
				m_command_result.push(data);
			}
			else
			{
				handle_rob_resources();
			}
			break;
		case CatanUtils::ServerInfo::GAME_STARTED:
			m_is_game_start = true;
			break;
		default:
			break;
		}
	}
}

void CatanClient::handle_player()
{
	char choice = 0;
	while (!m_game_is_finished)
	{
		WaitForSingleObject(m_is_my_turn.get(), INFINITE);
		std::cout << "enter what you want to do:" << std::endl;
		std::cout << "1. build settlement" << std::endl;
		std::cout << "2. upgrade settlement to city" << std::endl;
		std::cout << "3. build edge" << std::endl;
		std::cout << "4. finish turn" << std::endl;
		std::cout << "5. roll dices" << std::endl;
		std::cout << "6. print resources" << std::endl;
		std::cout << "7. buy development card" << std::endl;
		std::cout << "8. use development card" << std::endl;
		std::cin >> choice;
		switch (choice)
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
			std::cout << "Tree: " << std::to_string(m_player->
				get_number_of_specific_resource_card(CatanUtils::ResourceType::TREE)) << std::endl;
			std::cout << "Sheep: " << std::to_string(m_player->
				get_number_of_specific_resource_card(CatanUtils::ResourceType::SHEEP)) << std::endl;
			std::cout << "Stone: " << std::to_string(m_player->
				get_number_of_specific_resource_card(CatanUtils::ResourceType::STONE)) << std::endl;
			std::cout << "Wheat: " << std::to_string(m_player->
				get_number_of_specific_resource_card(CatanUtils::ResourceType::WHEAT)) << std::endl;
			std::cout << "Clay: " << std::to_string(m_player->
				get_number_of_specific_resource_card(CatanUtils::ResourceType::CLAY)) << std::endl;
			continue;
		case '7':
			handle_buy_development_card();
			continue;
		case '8':
			handle_use_development_card();
			continue;
		default:
			continue;
		}
	}
}

void CatanClient::update_new_resources_info(const std::string& data)
{
	auto parsed_data = CatanUtils::StringUtils::split(data, "\n");

	m_player->update_resources(parsed_data.at(1));
	m_gui_client.update_available_resources(m_player->get_resource_cards());
}

void CatanClient::update_new_development_cards_info(const std::string& data)
{
	auto parsed_data = CatanUtils::StringUtils::split(data, "\n");

	m_player->update_development_cards(parsed_data.at(1));
	m_gui_client.update_available_development_cards(m_player->get_development_cards());
}

void CatanClient::update_dices(const std::string& data)
{
	const uint32_t first_dice = std::atoi(
		CatanUtils::StringUtils::split(data, ",").at(0).c_str());
	const uint32_t second_dice = std::atoi(
		CatanUtils::StringUtils::split(data, ",").at(1).c_str());

	std::cout << "The dice numbers are: " << std::to_string(first_dice) << "," <<
		std::to_string(second_dice) << std::endl;
	m_gui_client.update_dices(first_dice, second_dice);
}

void CatanClient::handle_buy_development_card()
{
	if (m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::STONE) < 1 ||
		m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::WHEAT) < 1 ||
		m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::SHEEP) < 1)
	{
		std::cout << "not enough source" << std::endl;
		return;
	}

	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::BUY_DEVELOPMENT_CARD));
	m_client.send_data(command.str());

	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::BUY_DEVELOPMENT_CARD_SUCCEEDED)))
	{
		std::cout << "could not buy development card with server info: " << server_result << std::endl;
		return;
	}
	std::cout << "bought the development card" << std::endl;
}

void CatanClient::handle_use_development_card()
{
	char choice = 0;
	std::cout << "enter what you want to do:" << std::endl;
	std::cout << "1. road building card" << std::endl;
	std::cout << "2. monopoly card" << std::endl;
	std::cout << "3. knight card" << std::endl;
	std::cout << "4. year of plenty card" << std::endl;
	std::cin >> choice;
	switch (choice)
	{
	case '1':
		handle_road_building_card();
		break;
	case '2':
		handle_monopoly_card();
		break;
	case '3':
		//handle_knight_card();
		break;
	case '4':
		handle_abundance_card();
		break;
	default:
		break;
	}
}

void CatanClient::handle_road_building_card()
{
	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::ROAD_BUILDING_CARD)) <<
		'\n';

	std::string row_number;
	std::string col_number;
	std::cout << "enter first row number: ";
	std::cin >> row_number;
	std::cout << "enter first col number: ";
	std::cin >> col_number;

	command << row_number << "," << col_number << "\n";

	std::cout << "enter second row number: ";
	std::cin >> row_number;
	std::cout << "enter second col number: ";
	std::cin >> col_number;

	command << row_number << "," << col_number << "\n";

	m_client.send_data(command.str());
	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::ROAD_BUILDING_CARD_SUCCEEDED)))
	{
		std::cout << "could not use the road building card with server info: " << server_result <<
			std::endl;
		return;
	}
	std::cout << "used the road building card" << std::endl;
}

void CatanClient::handle_monopoly_card()
{
	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::MONOPOLY_CARD)) <<
		'\n';

	std::string resource_type;
	std::cout << "enter resource type: ";
	std::cin >> resource_type;
	command << resource_type;

	m_client.send_data(command.str());
	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::MONOPOLY_CARD_SUCCEEDED)))
	{
		std::cout << "could not use the monopoly card with server info: " << server_result <<
			std::endl;
		return;
	}
	std::cout << "used the monopoly card" << std::endl;
}

void CatanClient::handle_abundance_card()
{
	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::ABUNDANCE_CARD)) <<
		'\n';

	std::string resource_type;
	std::cout << "enter first resource type: ";
	std::cin >> resource_type;
	
	command << resource_type << "\n";

	std::cout << "enter second resource type: ";
	std::cin >> resource_type;

	command << resource_type << "\n";

	m_client.send_data(command.str());
	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::ABUNDANCE_CARD_SUCCEEDED)))
	{
		std::cout << "could not use the abundance card with server info: " << server_result <<
			std::endl;
		return;
	}
	std::cout << "used the abundance card" << std::endl;
}

void CatanClient::handle_build_edge()
{
	if (m_is_game_start)
	{
		if (m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::TREE) < 1 ||
			m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::CLAY) < 1)
		{
			std::cout << "not enough source" << std::endl;
			return;
		}
	}
	else
	{
		m_is_my_turn.reset_event();
	}

	std::string row_number;
	std::string col_number;
	std::cout << "enter row number: ";
	std::cin >> row_number;
	std::cout << "enter col number: ";
	std::cin >> col_number;

	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::CREATE_EDGE)) << '\n';
	command << row_number << "," << col_number;

	m_client.send_data(command.str());
	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::CREATE_EDGE_SUCCEEDED)))
	{
		if (!m_is_game_start)
		{
			m_is_my_turn.set_event();
		}
		std::cout << "could not create the edge with server info: " << server_result << std::endl;
		return;
	}
	std::cout << "create the edge" << std::endl;
}

void CatanClient::handle_build_settlement()
{
	if (m_is_game_start)
	{
		if (m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::TREE) < 1 ||
			m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::CLAY) < 1 ||
			m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::WHEAT) < 1 ||
			m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::SHEEP) < 1)
		{
			std::cout << "not enough source" << std::endl;
			return;
		}
	}

	std::string row_number;
	std::string col_number;
	std::cout << "enter row number: ";
	std::cin >> row_number;
	std::cout << "enter col number: ";
	std::cin >> col_number;

	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::CREATE_SETTLEMENT)) <<
		'\n';
	command << row_number << "," << col_number;

	m_client.send_data(command.str());
	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::CREATE_SETTLEMENT_SUCCEEDED)))
	{
		std::cout << "could not create the settlement with server info: " << server_result << std::endl;
		return;
	}
	std::cout << "create the settlement" << std::endl;
}

void CatanClient::handle_finish_turn()
{
	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::FINISH_TURN)) << '\n';
	m_client.send_data(command.str());

	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::FINISH_TURN_SUCCEEDED)))
	{
		std::cout << "could not finish turn with server info: " << server_result << std::endl;
		return;
	}
	std::cout << "your turn is finished" << std::endl;

	m_is_my_turn.reset_event();
}

void CatanClient::handle_upgrade_settlement_to_city()
{
	if (m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::STONE) < 3 ||
		m_player->get_number_of_specific_resource_card(CatanUtils::ResourceType::WHEAT) < 2)
	{
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
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::CREATE_CITY)) << '\n';
	command << row_number << "," << col_number;

	m_client.send_data(command.str());
	const auto server_result = m_command_result.pop_and_front();

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::CREATE_CITY_SUCCEEDED)))
	{
		std::cout << "could not upgrade the settlement with server info: " << server_result << std::endl;
		return;
	}
	std::cout << "upgrade the settlement" << std::endl;
}

void CatanClient::handle_roll_dices()
{
	std::stringstream command;
	command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::ROLL_DICES)) << '\n';

	m_client.send_data(command.str());

	auto server_result = m_command_result.pop_and_front();

	if (server_result == std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::ROBBER)))
	{
		handle_rob_resources();
		server_result = m_command_result.pop_and_front();
		if (server_result != std::to_string(
			static_cast<uint32_t>(CatanUtils::ServerInfo::KNIGHT)))
		{
			command.str(std::string());
			command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::EXIT_GAME));
			m_client.send_data(command.str());

			return;
		}
		handle_move_knight();
		server_result = m_command_result.pop_and_front();
	}

	if (server_result == std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::KNIGHT)))
	{
		handle_move_knight();
		server_result = m_command_result.pop_and_front();
	}

	if (server_result == std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::KNIGHT_ROB)))
	{
		handle_choose_player_to_rob();
		server_result = m_command_result.pop_and_front();
	}

	if (server_result != std::to_string(
		static_cast<uint32_t>(CatanUtils::ServerInfo::ROLL_DICES_SUCCEEDED)))
	{
		command.str(std::string());
		command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::EXIT_GAME));
		m_client.send_data(command.str());
		return;
	}

	std::cout << "roll the dice" << std::endl;
}

void CatanClient::handle_rob_resources()
{
	while (true)
	{
		auto chosen_resources_to_rob = choose_resources_to_rob();

		std::stringstream command;
		command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::ROB_RESOURCES)) <<
			'\n';
		command << chosen_resources_to_rob;

		m_client.send_data(command.str());

		const auto server_result = m_command_result.pop_and_front();
		if (server_result == std::to_string(
			static_cast<uint32_t>(CatanUtils::ServerInfo::ROB_RESOURCES_SUCCEEDED)))
		{
			return;
		}
		std::cout << "failed to rob with server info: " << server_result << std::endl;
	}
}

std::string CatanClient::choose_resources_to_rob() const
{
	// TODO make sure that this function does not return wrong number of resources
	std::stringstream chosen_resources_to_rob;
	std::string number_of_resources;

	for (const auto& [key, value] : m_player->get_resource_cards())
	{
		while (true)
		{
			std::cout << "enter how many" << convert_resource_type_to_string(key) << "to remove: ";
			std::cin >> number_of_resources;
			if (std::stoi(number_of_resources) >= 0 &&
				std::stoi(number_of_resources) <= m_player->get_number_of_specific_resource_card(key))
			{
				break;
			}
		}
		chosen_resources_to_rob << std::to_string(static_cast<uint32_t>(key)) << "," <<
			number_of_resources << ";";
	}
	return chosen_resources_to_rob.str();
}

void CatanClient::handle_move_knight()
{
	while (true)
	{
		std::string row_number;
		std::string col_number;
		std::cout << "enter row number: ";
		std::cin >> row_number;
		std::cout << "enter col number: ";
		std::cin >> col_number;

		std::stringstream command;
		command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::MOVE_KNIGHT)) <<
			'\n';
		command << row_number << "," << col_number;
		m_client.send_data(command.str());

		const auto server_result = m_command_result.pop_and_front();
		if (server_result == std::to_string(
			static_cast<uint32_t>(CatanUtils::ServerInfo::MOVE_KNIGHT_SUCCEEDED)))
		{
			break;
		}
		std::cout << "failed to move the knight with error: " << server_result << std::endl;
	}
}

void CatanClient::handle_choose_player_to_rob()
{
	while (true)
	{
		std::string player_number;
		std::cout << "enter player number: ";
		std::cin >> player_number;

		std::stringstream command;
		command << std::to_string(static_cast<uint32_t>(CatanUtils::ClientCommands::PLAYER_TO_ROB)) <<
			'\n';
		command << player_number;
		m_client.send_data(command.str());

		const auto server_result = m_command_result.pop_and_front();
		if (server_result == std::to_string(
			static_cast<uint32_t>(CatanUtils::ServerInfo::ROB_PLAYER_SUCCEEDED)))
		{
			break;
		}
		std::cout << "failed to rob player with error: " << server_result << std::endl;
	}
}

std::string convert_resource_type_to_string(const CatanUtils::ResourceType resource)
{
	switch (resource)
	{
	case CatanUtils::ResourceType::NONE:
		return "None ";
	case CatanUtils::ResourceType::WHEAT:
		return "Wheat ";
	case CatanUtils::ResourceType::CLAY:
		return "clay ";
	case CatanUtils::ResourceType::SHEEP:
		return "sheep ";
	case CatanUtils::ResourceType::TREE:
		return "trees ";
	case CatanUtils::ResourceType::STONE:
		return "stones ";
	default:
		return "";
	}
}
