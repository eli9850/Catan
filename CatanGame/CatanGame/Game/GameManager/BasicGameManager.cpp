#include "BasicGameManager.h"
#include "Exceptions/GameManagerExceptions.h"
#include "Utils/StringUtils.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>

BasicGameManager::BasicGameManager(const uint8_t number_of_players, const std::string& port_number):
	m_server(port_number), m_turn_number(0), m_game_started(false) {

	if (number_of_players < MIN_PLAYER_NUMBER || number_of_players > MAX_PLAYER_NUMBER) {
		throw InvalidPlayersNumber("You enter invalid number of players");
	}
	m_players.resize(number_of_players);
	for (auto  i = 0; i < number_of_players; i++) {
		m_players[i] = std::make_shared<Player>(static_cast<PlayerType>(i));
	}
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
	for (uint8_t i = 0; i < m_players.size(); i++) {
		std::thread temp(&BasicGameManager::handle_player ,this, i);
		players_threads.push_back(std::move(temp));
	}
	for (uint8_t i = 0; i < m_players.size(); i++) {
		players_threads[i].join();
	}
}

void BasicGameManager::handle_player(const uint8_t player_number) {
	
	initialize_player_start(player_number);

	std::string data;
	std::stringstream result_to_send;

	while (true) {
		data = m_server.recive_data(player_number);
		if (m_turn_number % m_players.size() != player_number) {
			result_to_send << std::to_string(static_cast<uint8_t>(CommandResult::NOT_YOUR_TURN));
			m_server.send_data(player_number, result_to_send.str());
		}
		else {
			auto result = handle_command(player_number, data);
			result_to_send << std::to_string(static_cast<uint8_t>(result));
			m_server.send_data(player_number, result_to_send.str());
		}
	}
}

void BasicGameManager::initialize_player_start(const uint8_t player_number) {

	wait_and_handle_command(player_number, m_turn_number, CommandType::SETTLEMENT, CommandResult::ONLY_SETTLEMENT);
	send_board_to_everyone();
	wait_and_handle_command(player_number, m_turn_number, CommandType::EDGE, CommandResult::ONLY_EDGE);
	send_board_to_everyone();
	m_turn_number++;
	wait_and_handle_command(player_number, m_players.size() - (m_turn_number % m_players.size()), CommandType::SETTLEMENT, CommandResult::ONLY_SETTLEMENT);
	send_board_to_everyone();
	wait_and_handle_command(player_number, m_players.size() - (m_turn_number % m_players.size()), CommandType::EDGE, CommandResult::ONLY_EDGE);
	send_board_to_everyone();
	m_turn_number++;
}

void BasicGameManager::wait_and_handle_command(const uint8_t player_number, const uint8_t player_turn, const CommandType command, const CommandResult command_in_failure){
	std::string data;
	std::stringstream result;
	while (true) {
		data = m_server.recive_data(player_number);
		if (m_turn_number != player_turn) {
			result << std::to_string(static_cast<uint8_t>(CommandResult::NOT_YOUR_TURN));
			m_server.send_data(player_number, result.str());
		}
		else {
			auto parsed_data = split(data, "\n");
			if (static_cast<CommandType>(stoi(parsed_data[0])) != command) {
				result << std::to_string(static_cast<uint8_t>(command_in_failure));
				m_server.send_data(player_number, result.str());
			}
			else {
				if (command == CommandType::SETTLEMENT) {
					result << std::to_string(static_cast<uint8_t>(handle_create_settlement(player_number, parsed_data)) - 1);
					m_server.send_data(player_number, result.str());
					break;
				}
				else if (command == CommandType::EDGE) {
					result << std::to_string(static_cast<uint8_t>(handle_create_edge(player_number, parsed_data)) - 1);
					m_server.send_data(player_number, result.str());
					break;
				}
			}
		}
	}
}

const BasicBoard& BasicGameManager::get_board() const {
	return m_board;
}

int8_t BasicGameManager::get_winner() const {
	for (uint8_t i = 0; i < m_players.size(); i++) {
		if (m_players[0]->get_number_of_points() >= 10) {
			return i;
		}
	}
	return -1;
}

CommandResult BasicGameManager::handle_command(const uint8_t player_number, const std::string& data) {
	auto parsed_data = split(data, "\n");
	switch (static_cast<CommandType>(std::stoi(parsed_data[0])))
	{
	case CommandType::SETTLEMENT:
		return handle_create_settlement(player_number, parsed_data);
		break;
	case CommandType::EDGE:
		return handle_create_edge(player_number, parsed_data);
		break;
	case CommandType::FINISH_TURN:
		m_turn_number++;
		break;
	default:
		throw UnknownCommand("This is an unknown command");
	}
}

CommandResult BasicGameManager::handle_create_edge(const uint8_t player_number, const std::vector<std::string> data) {
	auto edge_place = split(data[1], ",");
	uint8_t row_number = stoi(edge_place[0]);
	uint8_t column_number = stoi(edge_place[1]);
	if (is_possible_to_create_edge(static_cast<PlayerType>(player_number), row_number, column_number)) {
		if (m_players[player_number]->get_number_of_resource_cards(ResourceType::CLAY) < 1 ||
			m_players[player_number]->get_number_of_resource_cards(ResourceType::TREE) < 1) {
			return CommandResult::NOT_ENOUGH_RESOURCES;
		}
			m_board.create_edge(row_number, column_number, static_cast<PlayerType>(player_number));
			m_players[player_number]->decrease_resource_card(ResourceType::TREE);
			m_players[player_number]->decrease_resource_card(ResourceType::CLAY);
			return CommandResult::SUCCESS_WITH_INFO;
	}
	else {
		return CommandResult::INVALID_PLACE;
	}
	
}

CommandResult BasicGameManager::handle_create_settlement(const uint8_t player_number, const std::vector<std::string> data) {
	auto structure_place = split(data[1], ",");
	uint8_t row_number = stoi(structure_place[0]);
	uint8_t column_number = stoi(structure_place[1]);
	if (!m_game_started) {
		m_board.create_settlement(row_number, column_number, static_cast<PlayerType>(player_number));
		return CommandResult::SUCCESS_WITH_INFO;
	}
	else {
		if (is_possible_to_create_settlement(static_cast<PlayerType>(player_number), row_number, column_number)) {
			if (m_players[player_number]->get_number_of_resource_cards(ResourceType::CLAY) < 1 ||
				m_players[player_number]->get_number_of_resource_cards(ResourceType::WHEAT) < 1||
				m_players[player_number]->get_number_of_resource_cards(ResourceType::TREE) < 1||
				m_players[player_number]->get_number_of_resource_cards(ResourceType::SHEEP) < 1 ) {
				return CommandResult::NOT_ENOUGH_RESOURCES;
			}
			m_board.create_settlement(row_number, column_number, static_cast<PlayerType>(player_number));
			m_players[player_number]->decrease_resource_card(ResourceType::CLAY);
			m_players[player_number]->decrease_resource_card(ResourceType::WHEAT);
			m_players[player_number]->decrease_resource_card(ResourceType::TREE);
			m_players[player_number]->decrease_resource_card(ResourceType::SHEEP);
			return CommandResult::SUCCESS_WITH_INFO;
		}
		else {
			return CommandResult::INVALID_PLACE;
		}
	}
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
		if (adjacent_edges[i]->get_player() == player) {
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
		if (adjacent_nodes[i]->get_structure()->get_player().get_player_type() == player) {
			return true;
		}

	}
	auto adjacent_edges = m_board.get_edge_adjacent_edges(row_number, col_number);
	for (auto i = 0; i < adjacent_edges.size(); i++) {
		if (adjacent_edges[i]->get_player() == player) {
			return true;
		}
	}
	return false;
}

void BasicGameManager::send_board_to_everyone() const {
	for (uint8_t i = 0; i < m_players.size(); i++){
		m_server.send_data(i, m_board.to_string());
	}
}