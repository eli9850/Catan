#pragma once

#include <vector>

#include "Game/Board/Board.h"
#include "Game/Players/Players.h"

class GameManager final
{
public:
	explicit GameManager(const uint32_t number_of_players);
	void launch();

	// This private section is for methods
private:
	void get_players();
	void before_start();
	void play();
	bool is_there_a_winner() const;
	uint32_t roll_the_dice() const;
	
	// This private section is for members
private:
	uint32_t m_turn_number;
	Board m_game_board;
	std::vector<DevelopmentCards> m_development_cards;
	std::vector<std::shared_ptr<Player>> m_players;
};
