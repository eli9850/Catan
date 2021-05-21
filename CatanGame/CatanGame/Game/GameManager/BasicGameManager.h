#pragma once

#include <vector>
#include <memory>

#include "IGameManager.h"
#include "Game/Board/GameBoard/BasicBoard.h"
#include "Game/Players/Players.h"

constexpr int MIN_PLAYER_NUMBER = 3;
constexpr int MAX_PLAYER_NUMBER = 4;


class BasicGameManager: public IGameManager
{
public:
	BasicGameManager(const uint8_t number_of_players);

	void start_game() override;
	void wait_for_players();

	const BasicBoard& get_board() const;

private:
	std::vector<std::shared_ptr<Player>> m_players;
	BasicBoard m_board;
	uint8_t m_turn_number;
};

