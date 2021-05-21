#include "BasicGameManager.h"
#include "Exceptions/GameManagerExceptions.h"
#include "Utils/StringUtils.h"

BasicGameManager::BasicGameManager(const uint8_t number_of_players, const uint16_t port_number): 
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
	wait_for_players();


}

void BasicGameManager::wait_for_players() {
	for (auto  i = 0; i < m_players.size(); i++) {
		m_server.accept_client();
	}
}

void BasicGameManager::initialize_players_start() {
	std::string data;
	for (auto  i = 0; i < m_players.size(); i++) {
		data = m_server.recive_data(i);
		process_command(data);
		process_command(data);
	}
	for (auto i = m_players.size(); i >= 0 ; i--) {
		data = m_server.recive_data(i);
		process_command(data);
		process_command(data);
	}

}

void BasicGameManager::process_command(const std::string& data) {
	auto parsed_data = split(data, "\n");
	switch (static_cast<CommandType>(std::stoi(parsed_data[0])))
	{
	case CommandType::STRUCTURE:
		handle_create_structure(parsed_data);
		break;
	case CommandType::EDGE:
		handle_create_edge(parsed_data);
		break;
	default:
		throw UnknownCommand("This is an unknown command");
	}
	

	
}

const BasicBoard& BasicGameManager::get_board() const {
	return m_board;
}