#include "BasicGameManager.h"
#include "Exceptions/GameManagerExceptions.h"

BasicGameManager::BasicGameManager(const uint8_t number_of_players) {

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

}

const BasicBoard& BasicGameManager::get_board() const {
	return m_board;
}