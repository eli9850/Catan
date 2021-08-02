#include "BasicGameManager.h"
#include "Exceptions/GameManagerExceptions.h"
#include "Exceptions/BoardExceptions.h"
#include "Exceptions/PlayerExceptions.h"
#include "Utils/StringUtils.h"
#include "Utils/MapUtils.h"
#include "Utils/WinApi.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <random>

BasicGameManager::BasicGameManager(const uint8_t number_of_players, const std::string& port_number):
	m_server(port_number), m_turn_number(0), m_game_started(false), m_is_robbed_on(false){

	if (number_of_players < MIN_PLAYER_NUMBER || number_of_players > MAX_PLAYER_NUMBER) {
		//throw InvalidPlayersNumber("You enter invalid number of players");
	}
	m_players.resize(number_of_players);
	for (auto i = 0; i < number_of_players; i++) {
		m_players.at(i) = std::make_shared<Player>(static_cast<PlayerType>(i));
	}
	m_events.resize(number_of_players);
	m_board.create_board();
}

void BasicGameManager::start_game() {
	connect_players_and_start();
	std::cout << "The winner is: " << std::to_string(get_winner()) << std::endl;
}

void BasicGameManager::connect_players_and_start() {
	std::vector<std::thread> players_threads;
	for (uint8_t i = 0; i < m_players.size(); i++) {
		m_server.accept_client();	
	}
	std::cout << "all players connected" << std::endl;
	send_board_to_everyone();
	
	m_server.send_data(0, std::to_string(static_cast<uint8_t>(CommandResult::YOUR_TURN)));
	for (uint8_t i = 0; i < m_players.size(); i++) {
		std::thread temp(&BasicGameManager::handle_player ,this, i);
		players_threads.push_back(std::move(temp));
	}
	for (uint8_t i = 0; i < m_players.size(); i++) {
		players_threads.at(i).join();
	}
}

void BasicGameManager::handle_player(const uint8_t player_number) {
	
	// using srand for every player
	std::srand(std::chrono::system_clock::now().time_since_epoch().count());
	initialize_player_start(player_number);

	std::string data;
	std::stringstream result_to_send;

	while (true) {
		result_to_send.str(std::string());
		data = m_server.recive_data(player_number);
		if (m_turn_number % m_players.size() != player_number) {
			auto result = handle_command_when_not_your_turn(player_number, data);
			result_to_send << std::to_string(static_cast<uint8_t>(result));
			m_server.send_data(player_number, result_to_send.str());
		}
		else {
			auto result = handle_command(player_number, data);
			result_to_send << std::to_string(static_cast<uint8_t>(result));
			m_server.send_data(player_number, result_to_send.str());
			std::cout << "The result is: " << std::to_string(static_cast<uint8_t>(result)) << "\n\n";
		}
	}
}

void BasicGameManager::initialize_player_start(const uint8_t player_number) {

	wait_and_handle_command(player_number, player_number, CommandType::SETTLEMENT, CommandResult::ONLY_SETTLEMENT);
	send_board_to_everyone();
	wait_and_handle_command(player_number, player_number, CommandType::EDGE, CommandResult::ONLY_EDGE);
	send_board_to_everyone();
	pass_turn();
	wait_and_handle_command(player_number, ((m_players.size() - 1) - player_number) % m_players.size(), CommandType::SETTLEMENT, CommandResult::ONLY_SETTLEMENT);
	send_board_to_everyone();
	wait_and_handle_command(player_number, ((m_players.size() - 1) - player_number) % m_players.size(), CommandType::EDGE, CommandResult::ONLY_EDGE);
	send_board_to_everyone();
	pass_turn();
	if (player_number == 0) {
		m_game_started = true;
	}
}

void BasicGameManager::wait_and_handle_command(const uint8_t player_number, const uint8_t player_turn, const CommandType command, const CommandResult command_in_failure){
	std::string data;
	while (true) {
		std::stringstream result;
		data = m_server.recive_data(player_number);
		if (m_turn_number % m_players.size() != player_turn) {
			result << std::to_string(static_cast<uint8_t>(CommandResult::NOT_YOUR_TURN));
			m_server.send_data(player_number, result.str());
		}
		else {
			auto parsed_data = split(data, "\n");
			if (static_cast<CommandType>(stoi(parsed_data.at(0))) != command) {
				result << std::to_string(static_cast<uint8_t>(command_in_failure));
				m_server.send_data(player_number, result.str());
			}
			else {
				if (command == CommandType::SETTLEMENT) {
					auto create_result = handle_create_settlement(player_number, parsed_data);
					result << std::to_string(static_cast<uint8_t>(create_result));
					std::cout << "The result is: " << std::to_string(static_cast<uint8_t>(create_result)) << "\n\n";
					m_server.send_data(player_number, result.str());
					if (create_result == CommandResult::SUCCESS) {
						break;
					}
				}
				else if (command == CommandType::EDGE) {
					auto create_result = handle_create_edge(player_number, parsed_data);
					result << std::to_string(static_cast<uint8_t>(create_result));
					std::cout << "The result is: " << std::to_string(static_cast<uint8_t>(create_result)) << "\n\n";
					m_server.send_data(player_number, result.str());
					if (create_result == CommandResult::SUCCESS) {
						break;
					}
				}
			}
		}
	}
}

CommandResult BasicGameManager::handle_command(const uint8_t player_number, const std::string& data) {
	auto parsed_data = split(data, "\n");
	CommandResult result;
	switch (static_cast<CommandType>(std::stoi(parsed_data.at(0))))
	{
	case CommandType::SETTLEMENT:
		result = handle_create_settlement(player_number, parsed_data);
		if (result == CommandResult::SUCCESS) {
			send_board_to_everyone();
		}
		return result;
	case CommandType::EDGE:
		result = handle_create_edge(player_number, parsed_data);
		if (result == CommandResult::SUCCESS) {
			send_board_to_everyone();
		}
		return result;
	case CommandType::FINISH_TURN:
		pass_turn();
		return CommandResult::TURN_AS_FINISHED;
	case CommandType::CITY:
		result = handle_upgrade_settlement_to_city(player_number, parsed_data);
		if (result == CommandResult::SUCCESS) {
			send_board_to_everyone();
		}
		return result;
	case CommandType::ROLL_DICES:
		return handle_roll_dices(player_number, parsed_data);
	default:
		throw UnknownCommand("This is an unknown command");
	}
}

CommandResult BasicGameManager::handle_command_when_not_your_turn(const uint8_t player_number, const std::string& data) {
	
	auto parsed_data = split(data, "\n");
	CommandResult result;
	auto& resource_to_robbed = parsed_data.at(1);
	switch (static_cast<CommandType>(std::stoi(parsed_data.at(0))))
	{
	case CommandType::ROBBED_RESOURCES:
		if (!m_is_robbed_on) {
			return CommandResult::NOT_YOUR_TURN;
		}
		try {
			m_players.at(player_number)->rob_resources(resource_to_robbed);
		}
		catch (const WrongNumberOfRobbedResourceException& e) {
			return CommandResult::FAILED_TO_ROB;
		}	
		m_events.at(player_number).set_event();
		return CommandResult::SUCCESS;
	case CommandType::SETTLEMENT:
	case CommandType::EDGE:
	case CommandType::FINISH_TURN:
	case CommandType::CITY:
	case CommandType::ROLL_DICES:
		return CommandResult::NOT_YOUR_TURN;
	default:
		throw UnknownCommand("This is an unknown command");
	}
}

CommandResult BasicGameManager::handle_create_edge(const uint8_t player_number, const std::vector<std::string> data) {
	auto edge_place = split(data.at(1), ",");
	uint8_t row_number = stoi(edge_place.at(0));
	uint8_t column_number = stoi(edge_place.at(1));
	try {
		if (is_possible_to_create_edge(static_cast<PlayerType>(player_number), row_number, column_number)) {
			if (!m_game_started) {
				m_board.create_edge(row_number, column_number, static_cast<PlayerType>(player_number));
				return CommandResult::SUCCESS;
			}
			if (m_players.at(player_number)->get_number_of_resource_cards(ResourceType::CLAY) < 1 ||
				m_players.at(player_number)->get_number_of_resource_cards(ResourceType::TREE) < 1) {
				return CommandResult::NOT_ENOUGH_RESOURCES;
			}
			m_board.create_edge(row_number, column_number, static_cast<PlayerType>(player_number));
			m_players.at(player_number)->decrease_resource_card(ResourceType::TREE);
			m_players.at(player_number)->decrease_resource_card(ResourceType::CLAY);
			return CommandResult::SUCCESS;
		}
		else {
			return CommandResult::INVALID_PLACE;
		}
	} catch (const BoardError&) {
			return CommandResult::INVALID_PLACE;
	}
}

CommandResult BasicGameManager::handle_create_settlement(const uint8_t player_number, const std::vector<std::string> data) {
	auto structure_place = split(data.at(1), ",");
	uint8_t row_number = stoi(structure_place.at(0));
	uint8_t column_number = stoi(structure_place.at(1));
	if (!m_game_started) {
		try {
			if (m_board.get_node(row_number, column_number)->get_structure_type() != StructureType::NONE) {
				return CommandResult::INVALID_PLACE;
			}
			auto adjacent_nodes = m_board.get_node_adjacent_nodes(row_number, column_number);
			if (adjacent_nodes.size() != 0) {
				return CommandResult::INVALID_PLACE;
			}
			m_board.create_settlement(row_number, column_number, static_cast<PlayerType>(player_number));
			return CommandResult::SUCCESS;
						
		} catch (const BoardError&) {
			return CommandResult::INVALID_PLACE;
		}
		
	}
	else {
		try {
			if (is_possible_to_create_settlement(static_cast<PlayerType>(player_number), row_number, column_number)) {
				if (m_players.at(player_number)->get_number_of_resource_cards(ResourceType::CLAY) < 1 ||
					m_players.at(player_number)->get_number_of_resource_cards(ResourceType::WHEAT) < 1 ||
					m_players.at(player_number)->get_number_of_resource_cards(ResourceType::TREE) < 1 ||
					m_players.at(player_number)->get_number_of_resource_cards(ResourceType::SHEEP) < 1) {
					return CommandResult::NOT_ENOUGH_RESOURCES;
				}
				m_board.create_settlement(row_number, column_number, static_cast<PlayerType>(player_number));
				m_players.at(player_number)->decrease_resource_card(ResourceType::CLAY);
				m_players.at(player_number)->decrease_resource_card(ResourceType::WHEAT);
				m_players.at(player_number)->decrease_resource_card(ResourceType::TREE);
				m_players.at(player_number)->decrease_resource_card(ResourceType::SHEEP);
				return CommandResult::SUCCESS;
			}
			else {
				return CommandResult::INVALID_PLACE;
			}
		}
		catch (const BoardError&) {
			return CommandResult::INVALID_PLACE;
		}
	}
}

CommandResult BasicGameManager::handle_upgrade_settlement_to_city(const uint8_t player_number, const std::vector<std::string> data) {
	auto structure_place = split(data.at(1), ",");
	uint8_t row_number = stoi(structure_place.at(0));
	uint8_t column_number = stoi(structure_place.at(1));
	if (m_board.get_node(row_number, column_number)->get_structure() == nullptr) {
		return CommandResult::INVALID_PLACE;
	}
	try {
		if (static_cast<uint8_t>(m_board.get_node(row_number, column_number)->get_structure()->get_player()) != player_number) {
			return CommandResult::INVALID_PLACE;
		}
		if (m_players.at(player_number)->get_number_of_resource_cards(ResourceType::STONE) < 3 ||
			m_players.at(player_number)->get_number_of_resource_cards(ResourceType::WHEAT) < 2) {
			return CommandResult::NOT_ENOUGH_RESOURCES;
		}
		m_players.at(player_number)->decrease_resource_card(ResourceType::STONE);
		m_players.at(player_number)->decrease_resource_card(ResourceType::STONE);
		m_players.at(player_number)->decrease_resource_card(ResourceType::STONE);
		m_players.at(player_number)->decrease_resource_card(ResourceType::WHEAT);
		m_players.at(player_number)->decrease_resource_card(ResourceType::WHEAT);
		m_board.upgrade_settlement_to_city(row_number, column_number);
		return CommandResult::SUCCESS;
	} catch (const BoardError&) {
		return CommandResult::INVALID_PLACE;
	}
}

CommandResult BasicGameManager::handle_roll_dices(const uint8_t player_number, const std::vector<std::string> data) {
	uint8_t first_dice = 1 + rand() % 6;
	uint8_t second_dice = 1 + rand() % 6;
	uint32_t dice_number = first_dice + second_dice;

	std::cout << "The number of the dices is: " << std::to_string(dice_number) << std::endl;

	for (const auto& player : m_players) {
		std::stringstream data_to_send;
		data_to_send << std::to_string(static_cast<uint8_t>(CommandResult::DICES_NUMBERS)) << "\n";
		data_to_send << std::to_string(first_dice) << "," << std::to_string(second_dice) << "\n";
		m_server.send_data(static_cast<uint8_t>(player->get_player_type()), data_to_send.str());
	}

	if (dice_number == ROBBER_NUMBER) {
		return handle_robber(player_number, data);
	}

	for(const auto& player : m_players) {
		std::stringstream data_to_send;
		data_to_send << std::to_string(static_cast<uint8_t>(CommandResult::NEW_TURN_INFO)) << "\n";

		std::unordered_map<ResourceType, uint8_t> result;
		auto& board_nodes = m_board.get_nodes();
		for (uint8_t i = 0; i < board_nodes.size(); i++)
		{
			for (uint8_t j = 0; j < board_nodes.at(i).size(); j++)
			{
				if (board_nodes.at(i).at(j)->get_structure_type() != StructureType::NONE) {
					if (board_nodes.at(i).at(j)->get_structure()->get_player() == player->get_player_type()) {
						result = get_combine_maps(result, board_nodes.at(i).at(j)->get_structure()->get_resources(dice_number));
					}
				}
			}
		}
		for (const auto& key : result) {
			data_to_send << std::to_string(static_cast<uint8_t>(key.first)) << "," << std::to_string(key.second) << ";";
		}
		m_server.send_data(static_cast<uint8_t>(player->get_player_type()), data_to_send.str());
		m_players.at(static_cast<uint8_t>(player->get_player_type()))->combine_resources(result);
	}
	return CommandResult::SUCCESS;
}

CommandResult BasicGameManager::handle_robber(const uint8_t player_number, const std::vector<std::string> data) {
	
	m_is_robbed_on = true;
	for (const auto& player : m_players) {
		if (player->get_number_of_available_resources() > 7) {
			m_events.at(static_cast<uint8_t>(player->get_player_type())).reset_event();
			std::stringstream data_to_send;
			data_to_send << std::to_string(static_cast<uint8_t>(CommandResult::ROBBER));
			m_server.send_data(static_cast<uint8_t>(player->get_player_type()), data_to_send.str());
		}
	}
	if (m_players.at(player_number)->get_number_of_available_resources() > 7) {
		m_events.at(player_number).set_event();
		auto resource_to_robbed = m_server.recive_data(player_number);
		m_players.at(player_number)->rob_resources(resource_to_robbed.substr(resource_to_robbed.find('\n') + 1, resource_to_robbed.size() - resource_to_robbed.find('\n') + 1));
		std::stringstream data_to_send;
		data_to_send << std::to_string(static_cast<uint8_t>(CommandResult::SUCCESS));
		m_server.send_data(player_number, data_to_send.str());
	}
	
	WinUtils::wait_for_multiple_objects(m_events.size(), m_events, TRUE, INFINITE);
	m_is_robbed_on = false;

	move_knight(player_number
	);

	return CommandResult::SUCCESS;
}

void BasicGameManager::move_knight(const uint8_t player_number) {
	std::stringstream data_to_send;
	data_to_send << std::to_string(static_cast<uint8_t>(CommandType::MOVE_KNIGHT));
	m_server.send_data(player_number, data_to_send.str());
	auto knight_position_str = m_server.recive_data(player_number);
	std::pair<uint32_t, uint32_t> knight_position(std::stoi(split(knight_position_str, ",").at(0)), std::stoi(split(knight_position_str, ",").at(1)));
	m_board.set_robber_number(knight_position);

	send_board_to_everyone();

	data_to_send.str("");
	data_to_send << std::to_string(static_cast<uint8_t>(CommandResult::SUCCESS));
	m_server.send_data(player_number, data_to_send.str());
}

bool BasicGameManager::is_possible_to_create_settlement(const PlayerType player, const uint8_t row_number, const uint8_t col_number) const {

	if (m_board.get_node(row_number,col_number)->get_structure_type() != StructureType::NONE) {
		return false;
	}
	auto adjacent_nodes = m_board.get_node_adjacent_nodes(row_number, col_number);
	if (adjacent_nodes.size() != 0) {
		return false;
	}
	auto adjacent_edges = m_board.get_node_adjacent_edges(row_number, col_number);
	for (auto i = 0; i < adjacent_edges.size(); i++) {
		if (adjacent_edges.at(i)->get_player() == player) {
			return true;
		}
	}

	
	return true;
}

bool BasicGameManager::is_possible_to_create_edge(const PlayerType player, const uint8_t row_number, const uint8_t col_number) const {

	if (m_board.get_edge(row_number, col_number) != nullptr) {
		return false;
	}
	auto adjacent_nodes = m_board.get_edge_adjacent_nodes(row_number, col_number);
	for (auto i = 0; i < adjacent_nodes.size(); i++)
	{
		if (adjacent_nodes.at(i)->get_structure()->get_player() == player) {
			return true;
		}

	}
	auto adjacent_edges = m_board.get_edge_adjacent_edges(row_number, col_number);
	for (auto i = 0; i < adjacent_edges.size(); i++) {
		if (adjacent_edges.at(i)->get_player() == player) {
			return true;
		}
	}
	return false;
}

void BasicGameManager::send_board_to_everyone() const {
	
	auto board = m_board.to_string();
	std::cout << "The board now is\n" << board << "\n\n";
	
	std::stringstream data;
	data << std::to_string(static_cast<uint8_t>(CommandResult::INFO)) << "\n";
	data << board;
	// TODO: return also number of roads number of cards number of points and number of dev cards in this command
	for (uint8_t i = 0; i < m_players.size(); i++){
		m_server.send_data(i, data.str());
	}
}

void BasicGameManager::pass_turn() {
	m_turn_number++;
	if (m_turn_number >= m_players.size() && m_turn_number < 2 * m_players.size()) {
		auto a = ((m_players.size() - 1) - (m_turn_number % m_players.size())) % m_players.size();
		m_server.send_data(a,
			std::to_string(static_cast<uint8_t>(CommandResult::YOUR_TURN)));
	}
	else {
		m_server.send_data((m_turn_number) % m_players.size(),
			std::to_string(static_cast<uint8_t>(CommandResult::YOUR_TURN)));
	}
}

const BasicBoard& BasicGameManager::get_board() const {
	return m_board;
}

int8_t BasicGameManager::get_winner() const {
	for (uint8_t i = 0; i < m_players.size(); i++) {
		if (m_players.at(0)->get_number_of_points() >= 10) {
			return i;
		}
	}
	return -1;
}