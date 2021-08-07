#include "BasicGameManager.h"

#include <iostream>
#include <thread>
#include <sstream>
#include <set>


#include "Exceptions/BoardExceptions.h"
#include "Exceptions/GameManagerExceptions.h"
#include "Utils/MapUtils.h"
#include "Utils/StringUtils.h"
#include "Utils/TimeUtils.h"
#include "WinWrapers/WinUtils.h"
#include "Utils/RandomUtils.h"

constexpr uint32_t MIN_PLAYER_NUMBER = 3;
constexpr uint32_t MAX_PLAYER_NUMBER = 4;
constexpr uint32_t ROBBER_NUMBER = 7;
constexpr uint32_t NUMBER_TO_ROB_FROM = 8;
constexpr uint32_t COMMAND_TYPE_INDEX = 0;

BasicGameManager::BasicGameManager(const uint32_t number_of_players, const std::string& port_number):
	m_server(port_number), m_current_development_card(0), m_game_started(false), m_turn_number(0),
	m_is_robbed_on(false)
{
	if (number_of_players < MIN_PLAYER_NUMBER || number_of_players > MAX_PLAYER_NUMBER)
	{
		throw InvalidPlayersNumber("You enter invalid number of players");
	}
	m_players.resize(number_of_players);
	for (uint32_t i = 0; i < number_of_players; i++)
	{
		m_players.at(i) = std::make_shared<CatanUtils::Player>(static_cast<CatanUtils::PlayerType>(i));
		m_events.emplace_back(CatanUtils::WinUtils::Event(true, true));
	}
	m_board.create_board();

	uint32_t current_card = 0;
	for (size_t i = 0; i < CatanUtils::Consts::NUMBER_OF_KNIGHT_CARDS; i++)
	{
		m_development_cards.at(current_card) = CatanUtils::DevelopmentCards::KNIGHT_CARD;
		++current_card;
	}
	for (size_t i = 0; i < CatanUtils::Consts::NUMBER_OF_MONOPOLY_CARDS; i++)
	{
		m_development_cards.at(current_card) = CatanUtils::DevelopmentCards::MONOPOL_CARD;
		++current_card;
	}
	for (size_t i = 0; i < CatanUtils::Consts::NUMBER_OF_POINT_CARDS; i++)
	{
		m_development_cards.at(current_card) = CatanUtils::DevelopmentCards::POINT_CARD;
		++current_card;
	}
	for (size_t i = 0; i < CatanUtils::Consts::NUMBER_OF_YEAR_OF_PLENTY_CARDS; i++)
	{
		m_development_cards.at(current_card) = CatanUtils::DevelopmentCards::ABUNDANCE_CARD;
		++current_card;
	}
	for (size_t i = 0; i < CatanUtils::Consts::NUMBER_OF_ROAD_BUILDING_CARDS; i++)
	{
		m_development_cards.at(current_card) = CatanUtils::DevelopmentCards::ROAD_CARD;
		++current_card;
	}

	CatanUtils::RandomUtils::shuffle_array(m_development_cards);
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

	Sleep(1000);

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
		std::stringstream command;
		command << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::GAME_STARTED));
		m_server.send_data(player_number, command.str());
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
		if ((m_players.size() - 1) - (m_turn_number % m_players.size()) % m_players.size() != player_turn
		)
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
		if (resource->get_resource_type() != CatanUtils::ResourceType::NONE)
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
	}

	return settlement_resources;
}

void BasicGameManager::create_second_edge(const uint32_t player_number, const uint32_t player_turn)
{
	while (true)
	{
		std::stringstream result;
		auto data = m_server.receive_data(player_number);
		if ((m_players.size() - 1) - (m_turn_number % m_players.size()) % m_players.size() != player_turn
		)
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
	case CatanUtils::ClientCommands::BUY_DEVELOPMENT_CARD:
		result = handle_buy_development_card(player_number);
		if (result == CatanUtils::ServerInfo::BUY_DEVELOPMENT_CARD_SUCCEEDED)
		{
			send_player_resources(player_number);
			send_player_development_cards(player_number);
			send_board_to_everyone();
		}
		return result;

	case CatanUtils::ClientCommands::ROAD_BUILDING_CARD:
		result = handle_road_building_card(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::ROAD_BUILDING_CARD_SUCCEEDED)
		{
			send_player_development_cards(player_number);
			send_board_to_everyone();
		}
		return result;

	case CatanUtils::ClientCommands::ABUNDANCE_CARD:
		result = handle_abundance_card(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::ABUNDANCE_CARD_SUCCEEDED)
		{
			send_player_development_cards(player_number);
			send_player_resources(player_number);
			send_board_to_everyone();
		}
		return result;

	case CatanUtils::ClientCommands::MONOPOLY_CARD:
		result = handle_monopoly_card(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::MONOPOLY_CARD_SUCCEEDED)
		{
			send_player_development_cards(player_number);
			for (const auto& player : m_players)
			{
				send_player_resources(static_cast<uint32_t>(player->get_player_type()));
			}
			send_board_to_everyone();
		}
		return result;

	case CatanUtils::ClientCommands::KNIGHT_CARD:
		result = handle_knight_card(player_number, parsed_data);
		if (result == CatanUtils::ServerInfo::KNIGHT_CARD_SUCCEEDED)
		{
			send_player_development_cards(player_number);
			send_board_to_everyone();
		}
		return result;

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
	send_player_resources(player_number);
	return CatanUtils::ServerInfo::ROB_RESOURCES_SUCCEEDED;
}

CatanUtils::ServerInfo BasicGameManager::handle_buy_development_card(const uint32_t player_number)
{
	if (m_players.at(player_number)->get_number_of_specific_resource_card(
			CatanUtils::ResourceType::STONE) < 1 ||
		m_players.at(player_number)->get_number_of_specific_resource_card(
			CatanUtils::ResourceType::SHEEP) < 1 ||
		m_players.at(player_number)->get_number_of_specific_resource_card(
			CatanUtils::ResourceType::WHEAT) < 1)
	{
		return CatanUtils::ServerInfo::NOT_ENOUGH_RESOURCES;
	}
	if (m_current_development_card >= CatanUtils::Consts::NUMBER_OF_DEVELOPMENTS_CARDS)
	{
		return CatanUtils::ServerInfo::NO_MORE_DEVELOPMENT_CARDS;
	}
	m_players.at(player_number)->increase_development_card(
		m_development_cards.at(m_current_development_card));
	m_current_development_card++;

	m_players.at(player_number)->remove_from_specific_resource_card(
		CatanUtils::ResourceType::SHEEP, 1);
	m_players.at(player_number)->remove_from_specific_resource_card(
		CatanUtils::ResourceType::STONE, 1);
	m_players.at(player_number)->remove_from_specific_resource_card(
		CatanUtils::ResourceType::WHEAT, 1);
	return CatanUtils::ServerInfo::BUY_DEVELOPMENT_CARD_SUCCEEDED;
}

CatanUtils::ServerInfo BasicGameManager::handle_road_building_card(
	const uint32_t player_number, const std::vector<std::string>& data)
{
	if (m_players.at(player_number)->get_number_of_specific_development_card(
		CatanUtils::DevelopmentCards::ROAD_CARD) < 1)
	{
		return CatanUtils::ServerInfo::NOT_ENOUGH_DEVELOPMENT_CARDS;
	}

	const auto& first_road_str = CatanUtils::StringUtils::split(data.at(1), ",");
	const auto& second_road_str = CatanUtils::StringUtils::split(data.at(2), ",");

	const uint32_t first_row_number = std::stoi(first_road_str.at(0));
	const uint32_t first_column_number = std::stoi(first_road_str.at(1));
	const uint32_t second_row_number = std::stoi(second_road_str.at(0));
	const uint32_t second_column_number = std::stoi(second_road_str.at(1));

	try
	{
		if (!is_possible_to_create_edge(static_cast<CatanUtils::PlayerType>(player_number),
		                                first_row_number, first_column_number))
		{
			return CatanUtils::ServerInfo::INVALID_EDGE_PLACE;
		}
		else
		{
			m_board.create_edge(first_row_number, first_column_number,
			                    static_cast<CatanUtils::PlayerType>(player_number));
		}
		if (!is_possible_to_create_edge(static_cast<CatanUtils::PlayerType>(player_number),
		                                second_row_number, second_column_number))
		{
			return CatanUtils::ServerInfo::INVALID_EDGE_PLACE;
		}
		else
		{
			m_board.create_edge(second_row_number, second_column_number,
			                    static_cast<CatanUtils::PlayerType>(player_number));
		}
		m_players.at(player_number)->decrease_development_card(CatanUtils::DevelopmentCards::ROAD_CARD);
		return CatanUtils::ServerInfo::ROAD_BUILDING_CARD_SUCCEEDED;
	}
	catch (const BoardError&)
	{
		return CatanUtils::ServerInfo::INVALID_EDGE_PLACE;
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_monopoly_card(
	const uint32_t player_number, const std::vector<std::string>& data)
{
	if (m_players.at(player_number)->get_number_of_specific_development_card(
		CatanUtils::DevelopmentCards::MONOPOL_CARD) < 1)
	{
		return CatanUtils::ServerInfo::NOT_ENOUGH_DEVELOPMENT_CARDS;
	}
	try
	{
		const auto& resource_type = static_cast<CatanUtils::ResourceType>(std::stoi(data.at(1)));
		uint32_t number_of_resource_to_add = 0;

		for (const auto& player : m_players)
		{
			const auto number_of_resource_type = player->get_number_of_specific_resource_card(
				resource_type);
			number_of_resource_to_add += number_of_resource_type;
			player->remove_from_specific_resource_card(resource_type, number_of_resource_type);
		}

		m_players.at(player_number)->add_to_specific_resource_card(
			resource_type, number_of_resource_to_add);

		m_players.at(player_number)->decrease_development_card(
			CatanUtils::DevelopmentCards::MONOPOL_CARD);
		return CatanUtils::ServerInfo::MONOPOLY_CARD_SUCCEEDED;
	}
	catch (const BoardError&)
	{
		return CatanUtils::ServerInfo::INVALID_RESOURCE_TYPE;
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_abundance_card(const uint32_t player_number,
                                                               const std::vector<std::string>& data)
{
	if (m_players.at(player_number)->get_number_of_specific_development_card(
		CatanUtils::DevelopmentCards::ABUNDANCE_CARD) < 1)
	{
		return CatanUtils::ServerInfo::NOT_ENOUGH_DEVELOPMENT_CARDS;
	}
	try
	{
		const auto& first_resource_type = static_cast<CatanUtils::ResourceType>(std::stoi(data.at(1)));
		const auto& second_resource_type = static_cast<CatanUtils::ResourceType>(std::stoi(data.at(2)));

		m_players.at(player_number)->add_to_specific_resource_card(first_resource_type, 1);
		m_players.at(player_number)->add_to_specific_resource_card(second_resource_type, 1);

		m_players.at(player_number)->decrease_development_card(
			CatanUtils::DevelopmentCards::ABUNDANCE_CARD);
		return CatanUtils::ServerInfo::ABUNDANCE_CARD_SUCCEEDED;
	}
	catch (const BoardError&)
	{
		return CatanUtils::ServerInfo::INVALID_RESOURCE_TYPE;
	}
}

CatanUtils::ServerInfo BasicGameManager::handle_knight_card(
	const uint32_t player_number, const std::vector<std::string>& data)
{
	if (m_players.at(player_number)->get_number_of_specific_development_card(
		CatanUtils::DevelopmentCards::KNIGHT_CARD) < 1)
	{
		return CatanUtils::ServerInfo::NOT_ENOUGH_DEVELOPMENT_CARDS;
	}

	const auto& robber_place_str = CatanUtils::StringUtils::split(data.at(1), ",");

	const uint32_t row_number = std::stoi(robber_place_str.at(0));
	const uint32_t column_number = std::stoi(robber_place_str.at(1));
	const std::pair<uint32_t, uint32_t> new_robber_position(row_number, column_number);

	try
	{
		m_board.set_robber_position(new_robber_position);
		rob_with_knight(player_number);
		
		m_players.at(player_number)->decrease_development_card(CatanUtils::DevelopmentCards::KNIGHT_CARD);
		m_players.at(player_number)->increase_number_of_knights();
		return CatanUtils::ServerInfo::KNIGHT_CARD_SUCCEEDED;
	}
	catch (const BoardError&)
	{
		return CatanUtils::ServerInfo::INVALID_RESOURCE_PLACE;
	}
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
				m_players.at(player_number)->remove_from_specific_resource_card(
					CatanUtils::ResourceType::CLAY, 1);
				m_players.at(player_number)->remove_from_specific_resource_card(
					CatanUtils::ResourceType::WHEAT, 1);
				m_players.at(player_number)->remove_from_specific_resource_card(
					CatanUtils::ResourceType::TREE, 1);
				m_players.at(player_number)->remove_from_specific_resource_card(
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

	try
	{
		if (m_board.get_node(row_number, column_number)->get_structure() == nullptr)
		{
			return CatanUtils::ServerInfo::INVALID_STRUCTURE_PLACE;
		}

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
		send_player_resources(static_cast<uint32_t>(player->get_player_type()));
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
	}

	CatanUtils::WinUtils::wait_for_multiple_objects(m_events, true, INFINITE);
	m_is_robbed_on = false;

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
			catch (const InvalidResourceIndex&)
			{
				data_to_send.str("");
				data_to_send << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::INVALID_RESOURCE_PLACE));
				m_server.send_data(player_number, data_to_send.str());
			}
		}
	}

	rob_with_knight(player_number);

	send_board_to_everyone();
}

void BasicGameManager::rob_with_knight(const uint32_t player_number)
{
	const auto& [robber_row, robber_col] = m_board.get_robber_position();
	auto nodes_on_robber_position = m_board.get_resource_adjacent_nodes(robber_row, robber_col);
	std::set<CatanUtils::PlayerType> player_on_robber_resource;
	for (const auto& node : nodes_on_robber_position)
	{
		if (node->get_structure() != nullptr)
		{
			if (node->get_structure()->get_player() !=
				static_cast<CatanUtils::PlayerType>(player_number))
			{
				if (m_players.at(static_cast<uint32_t>(node->get_structure()->get_player()))->
				              get_number_of_available_resources() != 0)
				{
					player_on_robber_resource.emplace(node->get_structure()->get_player());
				}
			}
		}
	}

	if (player_on_robber_resource.empty())
	{
		return;
	}

	if (player_on_robber_resource.size() == 1)
	{
		auto& player_to_rob = m_players.at(static_cast<uint32_t>(*player_on_robber_resource.begin()));
		const auto resource_to_rob = player_to_rob->get_random_resource();
		player_to_rob->remove_from_specific_resource_card(resource_to_rob, 1);
		m_players.at(player_number)->add_to_specific_resource_card(resource_to_rob, 1);
		send_player_resources(player_number);
		send_player_resources(static_cast<uint32_t>(player_to_rob->get_player_type()));
		return;
	}

	std::stringstream data_to_send;
	data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::KNIGHT_ROB));
	m_server.send_data(player_number, data_to_send.str());

	while (true)
	{
		auto player_to_rob_command = m_server.receive_data(player_number);
		auto parsed_data = CatanUtils::StringUtils::split(player_to_rob_command, "\n");
		if (static_cast<CatanUtils::ClientCommands>(std::stoi(parsed_data.at(0))) !=
			CatanUtils::ClientCommands::PLAYER_TO_ROB)
		{
			data_to_send.str(std::string());
			data_to_send << std::to_string(
				static_cast<uint32_t>(CatanUtils::ServerInfo::ONLY_PLAYER_TO_ROB));
			m_server.send_data(player_number, data_to_send.str());
		}
		else
		{
			const auto& player_to_rob = static_cast<CatanUtils::PlayerType>
				(std::stoi(parsed_data.at(1)));
			if (player_on_robber_resource.find(player_to_rob) == player_on_robber_resource.end())
			{
				data_to_send.str("");
				data_to_send << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::INVALID_PLAYER_TO_ROB));
				m_server.send_data(player_number, data_to_send.str());
			}
			else
			{
				auto& player_to_rob_from = m_players.at(static_cast<uint32_t>(player_to_rob));
				const auto resource_to_rob = player_to_rob_from->get_random_resource();
				player_to_rob_from->remove_from_specific_resource_card(resource_to_rob, 1);
				m_players.at(player_number)->add_to_specific_resource_card(resource_to_rob, 1);
				send_player_resources(player_number);
				send_player_resources(static_cast<uint32_t>(player_to_rob_from->get_player_type()));
				data_to_send.str(std::string());
				data_to_send << std::to_string(
					static_cast<uint32_t>(CatanUtils::ServerInfo::ROB_PLAYER_SUCCEEDED));
				m_server.send_data(player_number, data_to_send.str());
				return;
			}
		}
	}
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

void BasicGameManager::send_player_development_cards(const uint32_t player_number) const
{
	std::stringstream data_to_send;
	data_to_send << std::to_string(static_cast<uint32_t>(CatanUtils::ServerInfo::NEW_DEVELOPMENT_CARDS))
		<< "\n";
	data_to_send << m_players.at(player_number)->get_development_cards_str();
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
