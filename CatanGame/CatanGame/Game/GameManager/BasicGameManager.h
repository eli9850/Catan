#pragma once

#include <vector>
#include <memory>

#include "IGameManager.h"
#include "Game/Board/GameBoard/BasicBoard.h"
#include "Server/Server.h"
#include "Game/Players/Players.h"

constexpr uint8_t MIN_PLAYER_NUMBER = 3;
constexpr uint8_t MAX_PLAYER_NUMBER = 4;
constexpr uint8_t ROBBER_NUMBER = 7;

enum class CommandType {
	FINISH_TURN,
	SETTLEMENT,
	CITY,
	EDGE,
	ROLL_DICES,
};

enum class CommandResult {
	SUCCESS,
	INFO,
	NEW_TURN_INFO,
	YOUR_TURN,
	NOT_YOUR_TURN,
	ONLY_SETTLEMENT,
	ONLY_EDGE,
	NOT_ENOUGH_RESOURCES,
	INVALID_PLACE,
	TURN_AS_FINISHED,
};

class BasicGameManager: public IGameManager
{
public:
	BasicGameManager(const uint8_t number_of_players, const std::string& port_number);

	// function for the start of the game
	void start_game() override;
	void connect_players_and_start();
	void handle_player(const uint8_t player_number);
	void initialize_player_start(const uint8_t player_number);
	void wait_and_handle_command(const uint8_t player_number, const uint8_t player_turn, const CommandType command, const CommandResult command_in_failure);

	// function for catan commands
	CommandResult handle_command(const uint8_t player_number, const std::string& data);
	CommandResult handle_command_when_not_your_turn(const uint8_t player_number, const std::string& data);
	CommandResult handle_create_edge(const uint8_t player_number, const std::vector<std::string> data);
	CommandResult handle_create_settlement(const uint8_t player_number, const std::vector<std::string> data);
	CommandResult handle_upgrade_settlement_to_city(const uint8_t player_number, const std::vector<std::string> data);
	CommandResult handle_roll_dices(const uint8_t player_number, const std::vector<std::string> data);
	CommandResult handle_robber(const uint8_t player_number, const std::vector<std::string> data);
	
	// functions for help
	bool is_possible_to_create_settlement(const PlayerType player, const uint8_t row_number, const uint8_t col_number) const;
	bool is_possible_to_create_edge(const PlayerType player, const uint8_t row_number, const uint8_t col_number) const;
	void send_board_to_everyone() const;
	void pass_turn();
	const BasicBoard& get_board() const;
	int8_t get_winner() const;

private:
	Server m_server;
	bool m_game_started;
	std::vector<std::shared_ptr<Player>> m_players;
	BasicBoard m_board;
	uint8_t m_turn_number;
	// TODO: check if we already roll the dice, otherwise we can't pass the turn
	//bool m_rolled_dices;
};

