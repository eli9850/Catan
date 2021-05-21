#pragma once

#include <vector>
#include <memory>

#include "IGameManager.h"
#include "Game/Board/GameBoard/BasicBoard.h"
#include "Server/CatanServer.h"
#include "Game/Players/Players.h"

constexpr int MIN_PLAYER_NUMBER = 3;
constexpr int MAX_PLAYER_NUMBER = 4;


class BasicGameManager: public IGameManager
{
public:
	BasicGameManager(const uint8_t number_of_players, const uint16_t port_number);

	void start_game() override;
	const BasicBoard& get_board() const;

private:
	void wait_for_players();
	void initialize_players_start();
	void process_command(const std::string& data);
	void handle_create_structure(const std::vector<std::string> data);
	void handle_create_edge(const std::vector<std::string> data);

private:
	CatanServer m_server;
	bool m_game_started;
	std::vector<std::shared_ptr<Player>> m_players;
	BasicBoard m_board;
	uint8_t m_turn_number;
};

