#include "BasicGameManager.h"

#include <iostream>
#include <thread>
#include <sstream>


#include "Exceptions/BoardExceptions.h"
#include "Exceptions/GameManagerExceptions.h"
#include "Utils/MapUtils.h"
#include "Utils/StringUtils.h"
#include "Utils/TimeUtils.h"
#include "WinWrapers/WinUtils.h"

constexpr uint32_t MIN_PLAYER_NUMBER = 3;
constexpr uint32_t MAX_PLAYER_NUMBER = 4;
constexpr uint32_t ROBBER_NUMBER = 7;
constexpr uint32_t NUMBER_TO_ROB_FROM = 8;
constexpr uint32_t COMMAND_TYPE_INDEX = 0;

BasicGameManager::BasicGameManager(const uint32_t number_of_players, const std::string& port_number):
	m_server(port_number), m_game_started(false), m_turn_number(0), m_is_robbed_on(false)
{
	if (number_of_players < MIN_PLAYER_NUMBER || number_of_players > MAX_PLAYER_NUMBER)
	{
		throw InvalidPlayersNumber("You enter invalid number of players");
	}
	m_players.resize(number_of_players);
	for (uint32_t i = 0; i < number_of_players; i++)
	{
		m_players.at(i) = std::make_shared<CatanUtils::Player>(static_cast<CatanUtils::PlayerType>(i));
	}
	m_events.resize(number_of_players);
	m_board.create_board();
}

void BasicGameManager::start_game()
{
	connect_players_and_start();
}

void BasicGameManager::connect_players_and_start()
{
	std::vector<std::thread> players_threads;
	for (size_t i = 0; i < m_players.size(); i++)
	{
		m_server.accept_client();
		std::cout << "player number: " << i << " connected" << std::endl;
		std::stringstream data_to_send;
		data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::CONNECT_INFO)) <<
			"\n";
		data_to_send << std::to_string(i);
		m_server.send_data(i, data_to_send.str());
	}
	std::cout << "all players connected" << std::endl;
	send_board_to_everyone();

	m_server.send_data(0, std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::YOUR_TURN)));
	for (size_t i = 0; i < m_players.size(); i++)
	{
		std::thread temp(&BasicGameManager::handle_player, this, i);
		players_threads.push_back(std::move(temp));
	}
	for (size_t i = 0; i < m_players.size(); i++)
	{
		players_threads.at(i).join();
	}
}

void BasicGameManager::handle_player(const uint32_t player_number)
{
	// using srand for every player
	std::srand(CatanUtils::TimeUtils::get_time_since_epoch());

	initialize_player_start(player_number);

	std::stringstream result_to_send;

	while (true)
	{
		result_to_send.str(std::string());
		const auto data = m_server.receive_data(player_number);
		if (m_turn_number % m_players.size() != player_number)
		{
			result_to_send << std::to_string(
				static_cast<uint32_t>(handle_command_when_not_your_turn(player_number, data)));
			m_server.send_data(player_number, result_to_send.str());
		}
		else
		{
			result_to_send << std::to_string(
				static_cast<uint32_t>(handle_command(player_number, data)));
			m_server.send_data(player_number, result_to_send.str());
		}
	}
}

void BasicGameManager::initialize_player_start(const uint32_t player_number)
{
	create_first_settlement(player_number, player_number);
	send_board_to_everyone();

	create_first_edge(player_number, player_number);
	send_board_to_everyone();

	pass_turn();

	create_second_settlement(player_number, player_number);
	send_board_to_everyone();

	create_second_edge(player_number, player_number);
	send_board_to_everyone();

	pass_turn();

	if (player_number == 0)
	{
		m_game_started = true;
	}
}

void BasicGameManager::create_first_settlement(const uint32_t player_number, const uint32_t player_turn)
{
	while (true)
	{
		std::stringstream result;
		auto data = m_server.receive_data(player_number);
		if (m_turn_number % m_players.size() != player_turn)
		{
			result << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::NOT_YOUR_TURN));
			m_server.send_data(player_number, result.str());
		}
		else
		{
			auto parsed_data = CatanUtils::StringUtils::split(data, "\n");
			if (static_cast<CatanUtils::ClientCommands>(stoi(parsed_data.at(COMMAND_TYPE_INDEX))) !=
				CatanUtils::ClientCommands::CREATE_SETTLEMENT)
			{
				result << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::ONLY_SETTLEMENT));
				m_server.send_data(player_number, result.str());
			}
			else
			{
				auto create_result = handle_create_settlement(player_number, parsed_data);
				result << std::to_string(static_cast<uint32_t>(create_result));
				m_server.send_data(player_number, result.str());
				if (create_result == CatanUtils::ServerInfo::CREATE_SETTLEMENT_SUCCEEDED)
				{
					break;
				}
			}
		}
	}
}

void BasicGameManager::create_first_edge(const uint32_t player_number, const uint32_t player_turn)
{
	while (true)
	{
		std::stringstream result;
		auto data = m_server.receive_data(player_number);
		if (m_turn_number % m_players.size() != player_turn)
		{
			result << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::NOT_YOUR_TURN));
			m_server.send_data(player_number, result.str());
		}
		else
		{
			auto parsed_data = CatanUtils::StringUtils::split(data, "\n");
			if (static_cast<CatanUtils::ClientCommands>(stoi(parsed_data.at(COMMAND_TYPE_INDEX))) !=
				CatanUtils::ClientCommands::CREATE_EDGE)
			{
				result << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::ONLY_EDGE));
				m_server.send_data(player_number, result.str());
			}
			else
			{
				auto create_result = handle_create_edge(player_number, parsed_data);
				result << std::to_string(static_cast<uint32_t>(create_result));
				m_server.send_data(player_number, result.str());
				if (create_result == CatanUtils::ServerInfo::CREATE_EDGE_SUCCEEDED)
				{
					break;
				}
			}
		}
	}
}

void BasicGameManager::create_second_settlement(const uint32_t player_number, const uint32_t player_turn)
{
	while (true)
	{
		std::stringstream data_to_send;
		auto data = m_server.receive_data(player_number);
		if (m_turn_number % m_players.size() != player_turn)
		{
			data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::NOT_YOUR_TURN));
			m_server.send_data(player_number, data_to_send.str());
		}
		else
		{
			auto parsed_data = CatanUtils::StringUtils::split(data, "\n");
			if (static_cast<CatanUtils::ClientCommands>(stoi(parsed_data.at(COMMAND_TYPE_INDEX))) !=
				CatanUtils::ClientCommands::CREATE_SETTLEMENT)
			{
				data_to_send << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::ONLY_SETTLEMENT));
				m_server.send_data(player_number, data_to_send.str());
			}
			else
			{
				auto create_result = handle_create_settlement(player_number, parsed_data);
				data_to_send << std::to_string(static_cast<uint32_t>(create_result));
				m_server.send_data(player_number, data_to_send.str());
				if (create_result == CatanUtils::ServerInfo::CREATE_SETTLEMENT_SUCCEEDED)
				{
					const auto settlement_resources = get_settlement_resources(parsed_data.at(1));
					m_players.at(player_number)->combine_resources(settlement_resources);
					send_player_resources(player_number);
					break;
				}
			}
		}
	}
}

std::unordered_map<CatanUtils::ResourceType, uint32_t> BasicGameManager::get_settlement_resources(
	const std::string& position) const
{
	auto edge_place = CatanUtils::StringUtils::split(position, ",");
	const uint32_t row_number = std::stoi(edge_place.at(0));
	const uint32_t column_number = std::stoi(edge_place.at(1));

	std::unordered_map<CatanUtils::ResourceType, uint32_t> settlement_resources;

	auto resources = m_board.get_node_adjacent_resources(row_number, column_number);
	for (const auto& resource : resources)
	{
		if (settlement_resources.find(resource->get_resource_type()) != settlement_resources.end())
		{
			settlement_resources.at(resource->get_resource_type()) += 1;
		}
		else
		{
			settlement_resources.try_emplace(resource->get_resource_type(), 1);
		}
	}

	return settlement_resources;
}

void BasicGameManager::create_second_edge(const uint32_t player_number, const uint32_t player_turn)
{
	while (true)
	{
		std::stringstream result;
		auto data = m_server.receive_data(player_number);
		if (m_turn_number % m_players.size() != player_turn)
		{
			result << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::NOT_YOUR_TURN));
			m_server.send_data(player_number, result.str());
		}
		else
		{
			auto parsed_data = CatanUtils::StringUtils::split(data, "\n");
			if (static_cast<CatanUtils::ClientCommands>(stoi(parsed_data.at(COMMAND_TYPE_INDEX))) !=
				CatanUtils::ClientCommands::CREATE_EDGE)
			{
				result << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::ONLY_EDGE));
				m_server.send_data(player_number, result.str());
			}
			else
			{
				auto create_result = handle_create_edge(player_number, parsed_data);
				result << std::to_string(static_cast<uint32_t>(create_result));
				m_server.send_data(player_number, result.str());
				if (create_result == CatanUtils::ServerInfo::CREATE_EDGE_SUCCEEDED)
				{
					break;
				}
			}
		}
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_command(const uint32_t player_number,
                                                        const std::string& data)
{
	auto parsed_data = CatanUtils::StringUtils::split(data, "\n");
	CatanUtils::ServerInfo result;
	switch (static_cast<CatanUtils::ClientCommands>(std::stoi(parsed_data.at(0))))
	{
	case CatanUtils::ClientCommands::CREATE_SETTLEMENT:
		result = handle_create_settlement(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::CREATE_SETTLEMENT_SUCCEEDED)
		{
			send_player_resources(player_number);
			send_board_to_everyone();
		}
		return result;

	case CatanUtils::ClientCommands::CREATE_EDGE:
		result = handle_create_edge(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::CREATE_EDGE_SUCCEEDED)
		{
			send_player_resources(player_number);
			send_board_to_everyone();
		}
		return result;

	case CatanUtils::ClientCommands::FINISH_TURN:
		pass_turn();
		return CatanUtils::ServerInfo::FINISH_TURN_SUCCEEDED;

	case CatanUtils::ClientCommands::CREATE_CITY:
		result = handle_upgrade_settlement_to_city(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::CREATE_CITY_SUCCEEDED)
		{
			send_player_resources(player_number);
			send_board_to_everyone();
		}
		return result;

	case CatanUtils::ClientCommands::ROLL_DICES:
		result = handle_roll_dices(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::ROLL_DICES_SUCCEEDED)
		{
			send_board_to_everyone();
		}
		return result;
	default:
		return CatanUtils::ServerInfo::INVALID_COMMAND;
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_command_when_not_your_turn(
	const uint32_t player_number, const std::string& data)
{
	auto parsed_data = CatanUtils::StringUtils::split(data, "\n");
	auto& resource_to_robbed = parsed_data.at(1);

	CatanUtils::ServerInfo result;

	switch (static_cast<CatanUtils::ClientCommands>(std::stoi(parsed_data.at(0))))
	{
	case CatanUtils::ClientCommands::ROB_RESOURCES:
		if (!m_is_robbed_on)
		{
			return CatanUtils::ServerInfo::INVALID_COMMAND;
		}
		result = rob_resources_from_player(player_number, resource_to_robbed);
		if (result == CatanUtils::ServerInfo::ROB_RESOURCES_SUCCEEDED)
		{
			m_events.at(player_number).set_event();
		}
		return result;
	case CatanUtils::ClientCommands::CREATE_SETTLEMENT:
	case CatanUtils::ClientCommands::CREATE_EDGE:
	case CatanUtils::ClientCommands::FINISH_TURN:
	case CatanUtils::ClientCommands::CREATE_CITY:
	case CatanUtils::ClientCommands::ROLL_DICES:
	default:
		return CatanUtils::ServerInfo::NOT_YOUR_TURN;
	}
}

CatanUtils::ServerInfo BasicGameManager::rob_resources_from_player(
	const uint32_t player_number, const std::string& data)
{
	uint32_t number_of_resources_to_rob = 0;
	for (const auto& resource_str : CatanUtils::StringUtils::split(data, ";"))
	{
		const uint32_t value = std::stoi(CatanUtils::StringUtils::split(resource_str, ",").at(1));
		number_of_resources_to_rob += value;
		const auto resource_type = static_cast<CatanUtils::ResourceType>(std::stoi(
			CatanUtils::StringUtils::split(resource_str, ",").at(0)));
		if (m_players.at(player_number)->get_number_of_specific_resource_card(resource_type) < value)
		{
			return CatanUtils::ServerInfo::WRONG_NUMBER_OF_RESOURCES_TO_ROB;
		}
	}
	if (number_of_resources_to_rob != m_players.at(player_number)->
	                                            get_number_of_available_resources() / 2)
	{
		return CatanUtils::ServerInfo::WRONG_NUMBER_OF_RESOURCES_TO_ROB;
	}
	for (const auto& resource_str : CatanUtils::StringUtils::split(data, ";"))
	{
		const uint32_t value = std::stoi(CatanUtils::StringUtils::split(resource_str, ",").at(1));
		const auto resource_type = static_cast<CatanUtils::ResourceType>(std::stoi(
			CatanUtils::StringUtils::split(resource_str, ",").at(0)));
		m_players.at(player_number)->remove_from_specific_resource_card(resource_type, value);
	}
	return CatanUtils::ServerInfo::ROB_RESOURCES_SUCCEEDED;
}

CatanUtils::ServerInfo BasicGameManager::handle_create_edge(const uint32_t player_number,
                                                            const std::vector<std::string>& data)
{
	auto edge_place = CatanUtils::StringUtils::split(data.at(1), ",");
	const uint32_t row_number = std::stoi(edge_place.at(0));
	const uint32_t column_number = std::stoi(edge_place.at(1));
	try
	{
		if (is_possible_to_create_edge(static_cast<CatanUtils::PlayerType>(player_number), row_number,
		                               column_number))
		{
			if (!m_game_started)
			{
				m_board.create_edge(row_number, column_number,
				                    static_cast<CatanUtils::PlayerType>(player_number));
				return CatanUtils::ServerInfo::CREATE_EDGE_SUCCEEDED;
			}
			if (m_players.at(player_number)->get_number_of_specific_resource_card(
					CatanUtils::ResourceType::CLAY) < 1 ||
				m_players.at(player_number)->get_number_of_specific_resource_card(
					CatanUtils::ResourceType::TREE) < 1)
			{
				return CatanUtils::ServerInfo::NOT_ENOUGH_RESOURCES;
			}
			m_board.create_edge(row_number, column_number,
			                    static_cast<CatanUtils::PlayerType>(player_number));
			m_players.at(player_number)->remove_from_specific_resource_card(
				CatanUtils::ResourceType::TREE, 1);
			m_players.at(player_number)->remove_from_specific_resource_card(
				CatanUtils::ResourceType::CLAY, 1);
			return CatanUtils::ServerInfo::CREATE_EDGE_SUCCEEDED;
		}
		else
		{
			return CatanUtils::ServerInfo::INVALID_EDGE_PLACE;
		}
	}
	catch (const BoardError&)
	{
		return CatanUtils::ServerInfo::INVALID_EDGE_PLACE;
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_create_settlement(const uint32_t player_number,
                                                                  const std::vector<std::string>& data)
{
	auto structure_place = CatanUtils::StringUtils::split(data.at(1), ",");
	const uint32_t row_number = std::stoi(structure_place.at(0));
	const uint32_t column_number = std::stoi(structure_place.at(1));

	if (!m_game_started)
	{
		try
		{
			if (m_board.get_node(row_number, column_number)->get_structure_type() !=
				CatanUtils::StructureType::NONE)
			{
				return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
			}
			const auto adjacent_nodes = m_board.get_node_adjacent_nodes(row_number, column_number);
			if (!adjacent_nodes.empty())
			{
				return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
			}
			m_board.create_settlement(row_number, column_number,
			                          static_cast<CatanUtils::PlayerType>(player_number));
			return CatanUtils::ServerInfo::CREATE_SETTLEMENT_SUCCEEDED;
		}
		catch (const BoardError&)
		{
			return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
		}
	}
	else
	{
		try
		{
			if (is_possible_to_create_settlement(static_cast<CatanUtils::PlayerType>(player_number),
			                                     row_number,
			                                     column_number))
			{
				if (m_players.at(player_number)->get_number_of_specific_resource_card(
						CatanUtils::ResourceType::CLAY) < 1 ||
					m_players.at(player_number)->get_number_of_specific_resource_card(
						CatanUtils::ResourceType::WHEAT) < 1 ||
					m_players.at(player_number)->get_number_of_specific_resource_card(
						CatanUtils::ResourceType::TREE) < 1 ||
					m_players.at(player_number)->get_number_of_specific_resource_card(
						CatanUtils::ResourceType::SHEEP) < 1)
				{
					return CatanUtils::ServerInfo::NOT_ENOUGH_RESOURCES;
				}
				m_board.create_settlement(row_number, column_number,
				                          static_cast<CatanUtils::PlayerType>(player_number));
				m_players.at(player_number)->add_to_specific_resource_card(
					CatanUtils::ResourceType::CLAY, 1);
				m_players.at(player_number)->add_to_specific_resource_card(
					CatanUtils::ResourceType::WHEAT, 1);
				m_players.at(player_number)->add_to_specific_resource_card(
					CatanUtils::ResourceType::TREE, 1);
				m_players.at(player_number)->add_to_specific_resource_card(
					CatanUtils::ResourceType::SHEEP, 1);
				return CatanUtils::ServerInfo::CREATE_SETTLEMENT_SUCCEEDED;
			}
			else
			{
				return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
			}
		}
		catch (const BoardError&)
		{
			return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
		}
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_upgrade_settlement_to_city(
	const uint32_t player_number, const std::vector<std::string>& data)
{
	auto structure_place = CatanUtils::StringUtils::split(data.at(1), ",");
	const uint32_t row_number = std::stoi(structure_place.at(0));
	const uint32_t column_number = std::stoi(structure_place.at(1));

	if (m_board.get_node(row_number, column_number)->get_structure() == nullptr)
	{
		return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
	}
	try
	{
		if (static_cast<uint32_t>(m_board.get_node(row_number, column_number)->get_structure()->
		                                  get_player()) != player_number || m_board.
			get_node(row_number, column_number)->get_structure_type() !=
			CatanUtils::StructureType::SETTLEMENT)
		{
			return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
		}
		if (m_players.at(player_number)->get_number_of_specific_resource_card(
				CatanUtils::ResourceType::STONE) < 3 ||
			m_players.at(player_number)->get_number_of_specific_resource_card(
				CatanUtils::ResourceType::WHEAT) < 2)
		{
			return CatanUtils::ServerInfo::NOT_ENOUGH_RESOURCES;
		}
		m_players.at(player_number)->remove_from_specific_resource_card(
			CatanUtils::ResourceType::STONE, 3);
		m_players.at(player_number)->remove_from_specific_resource_card(
			CatanUtils::ResourceType::WHEAT, 2);
		m_board.upgrade_settlement_to_city(row_number, column_number);
		return CatanUtils::ServerInfo::CREATE_CITY_SUCCEEDED;
	}
	catch (const BoardError&)
	{
		return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_roll_dices(const uint32_t player_number,
                                                           const std::vector<std::string>& data)
{
	const uint32_t first_dice = 1 + rand() % 6;
	const uint32_t second_dice = 1 + rand() % 6;
	const auto dice_number = first_dice + second_dice;

	std::cout << "The number of the dices is: " << std::to_string(dice_number) << std::endl;

	for (const auto& player : m_players)
	{
		std::stringstream data_to_send;
		data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::DICES_NUMBERS)) <<
			"\n";
		data_to_send << std::to_string(first_dice) << "," << std::to_string(second_dice) << "\n";
		m_server.send_data(static_cast<uint32_t>(player->get_player_type()), data_to_send.str());
	}

	if (dice_number == ROBBER_NUMBER)
	{
		return handle_robber(player_number, data);
	}

	for (const auto& player : m_players)
	{
		std::unordered_map<CatanUtils::ResourceType, uint32_t> result;
		const auto& board_nodes = m_board.get_nodes();
		for (const auto& board_node_row : board_nodes)
		{
			for (const auto& board_node : board_node_row)
			{
				if (board_node->get_structure_type() != CatanUtils::StructureType::NONE)
				{
					if (board_node->get_structure()->get_player() == player->get_player_type())
					{
						result = CatanUtils::MapUtils::get_combine_maps(result,
							board_node->get_structure()->get_resources(dice_number));
					}
				}
			}
		}
		m_players.at(static_cast<uint32_t>(player->get_player_type()))->combine_resources(result);
		send_player_resources(player_number);
	}
	return CatanUtils::ServerInfo::ROLL_DICES_SUCCEEDED;
}

CatanUtils::ServerInfo BasicGameManager::handle_robber(const uint32_t player_number,
                                                       const std::vector<std::string>& data)
{
	m_is_robbed_on = true;
	for (const auto& player : m_players)
	{
		if (player->get_number_of_available_resources() >= NUMBER_TO_ROB_FROM)
		{
			m_events.at(static_cast<uint32_t>(player->get_player_type())).reset_event();
			std::stringstream data_to_send;
			data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::ROBBER));
			m_server.send_data(static_cast<uint32_t>(player->get_player_type()), data_to_send.str());
		}
	}
	if (m_players.at(player_number)->get_number_of_available_resources() >= NUMBER_TO_ROB_FROM)
	{
		m_events.at(player_number).set_event();
		while (true)
		{
			const auto resources_to_rob = m_server.receive_data(player_number);
			auto parsed_data = CatanUtils::StringUtils::split(resources_to_rob, "\n");
			if (static_cast<CatanUtils::ClientCommands>(std::stoi(parsed_data.at(0))) !=
				CatanUtils::ClientCommands::ROB_RESOURCES)
			{
				std::stringstream data_to_send;
				data_to_send << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::ONLY_ROBBER));
				m_server.send_data(player_number, data_to_send.str());
			}
			else
			{
				const auto result = rob_resources_from_player(player_number, parsed_data.at(1));
				std::stringstream data_to_send;
				data_to_send << std::to_string(static_cast<uint32_t>(result));
				m_server.send_data(player_number, data_to_send.str());
				if (result == CatanUtils::ServerInfo::ROB_RESOURCES_SUCCEEDED)
				{
					break;
				}
			}
		}

		CatanUtils::WinUtils::wait_for_multiple_objects(m_events, true, INFINITE);
		m_is_robbed_on = false;
	}

	move_knight(player_number);
	return CatanUtils::ServerInfo::ROLL_DICES_SUCCEEDED;
}

void BasicGameManager::move_knight(const uint32_t player_number)
{
	std::stringstream data_to_send;
	data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::KNIGHT));
	m_server.send_data(player_number, data_to_send.str());

	while (true)
	{
		const auto knight_position_data = m_server.receive_data(player_number);
		auto parsed_data = CatanUtils::StringUtils::split(knight_position_data, "\n");
		if (static_cast<CatanUtils::ClientCommands>(std::stoi(parsed_data.at(0))) !=
			CatanUtils::ClientCommands::MOVE_KNIGHT)
		{
			data_to_send.str(std::string());
			data_to_send << std::to_string(
				static_cast<uint32_t>(CatanUtils::ServerInfo::ONLY_KNIGHT));
			m_server.send_data(player_number, data_to_send.str());
		}
		else
		{
			const auto& knight_position_str = parsed_data.at(1);
			const std::pair<uint32_t, uint32_t> knight_position(
				std::stoi(CatanUtils::StringUtils::split(knight_position_str, ",").at(0)),
				std::stoi(CatanUtils::StringUtils::split(knight_position_str, ",").at(1)));
			try
			{
				m_board.set_robber_position(knight_position);
				data_to_send.str("");
				data_to_send << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::MOVE_KNIGHT_SUCCEEDED));
				m_server.send_data(player_number, data_to_send.str());
				break;
			}
			catch (const InvalidNodeIndex&)
			{
				data_to_send.str("");
				data_to_send << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::INVALID_RESOURCE_PLACE));
				m_server.send_data(player_number, data_to_send.str());
			}
		}
	}

	send_board_to_everyone();
}

bool BasicGameManager::is_possible_to_create_settlement(const CatanUtils::PlayerType player,
                                                        const uint32_t row_number,
                                                        const uint32_t col_number) const
{
	if (m_board.get_node(row_number, col_number)->get_structure_type() != CatanUtils::StructureType::NONE
	)
	{
		return false;
	}
	const auto adjacent_nodes = m_board.get_node_adjacent_nodes(row_number, col_number);
	if (!adjacent_nodes.empty())
	{
		return false;
	}
	const auto adjacent_edges = m_board.get_node_adjacent_edges(row_number, col_number);
	for (const auto& adjacent_edge : adjacent_edges)
	{
		if (adjacent_edge->get_player() == player)
		{
			return true;
		}
	}
	return false;
}

bool BasicGameManager::is_possible_to_create_edge(const CatanUtils::PlayerType player,
                                                  const uint32_t row_number,
                                                  const uint32_t col_number) const
{
	if (m_board.get_edge(row_number, col_number) != nullptr)
	{
		return false;
	}
	auto adjacent_nodes = m_board.get_edge_adjacent_nodes(row_number, col_number);
	for (auto& adjacent_node : adjacent_nodes)
	{
		if (adjacent_node->get_structure()->get_player() == player)
		{
			return true;
		}
	}
	auto adjacent_edges = m_board.get_edge_adjacent_edges(row_number, col_number);
	for (auto& adjacent_edge : adjacent_edges)
	{
		if (adjacent_edge->get_player() == player)
		{
			return true;
		}
	}
	return false;
}

void BasicGameManager::send_board_to_everyone() const
{
	const auto board = m_board.to_string();
	std::cout << "The board now is\n" << board << "\n\n";

	std::stringstream data;
	data << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::BOARD_INFO)) << "\n";
	data << board;
	// TODO: return also number of roads number of cards number of points and number of dev cards in this command
	for (size_t i = 0; i < m_players.size(); i++)
	{
		m_server.send_data(i, data.str());
	}
}

void BasicGameManager::send_player_resources(const uint32_t player_number) const
{
	std::stringstream data_to_send;
	data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::NEW_RESOURCES)) << "\n";
	data_to_send << m_players.at(player_number)->get_resources_str();
	m_server.send_data(player_number, data_to_send.str());
}

void BasicGameManager::pass_turn()
{
	m_turn_number++;
	if (m_turn_number >= m_players.size() && m_turn_number < 2 * m_players.size())
	{
		const auto opposite_turn_for_start =
			((m_players.size() - 1) - (m_turn_number % m_players.size())) % m_players.size();
		m_server.send_data(opposite_turn_for_start,
		                   std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::YOUR_TURN)));
	}
	else
	{
		m_server.send_data((m_turn_number) % m_players.size(),
		                   std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::YOUR_TURN)));
	}
}

const CatanUtils::BasicBoard& BasicGameManager::get_board() const
{
	return m_board;
}

int32_t BasicGameManager::get_winner() const
{
	for (size_t i = 0; i < m_players.size(); i++)
	{
		if (m_players.at(0)->get_number_of_points() >= 10)
		{
			return i;
		}
	}
	return -1;
}
