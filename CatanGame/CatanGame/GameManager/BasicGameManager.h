#pragma once

#include <vector>
#include <memory>

#include "IGameManager.h"
#include "Server/Server.h"

#include "Catan/Board/GameBoard/BasicBoard.h"
#include "Catan/Players/Players.h"
#include "WinWrapers/Events/Event.h"
#include "Catan/CatanConsts.h"

class BasicGameManager final : public IGameManager
{
public:
	BasicGameManager(const uint32_t number_of_players, const std::string& port_number);

	// function that start the game server
	void start_game() override;

private:
	void connect_players_and_start();
	void handle_player(const uint32_t player_number);
	void initialize_player_start(const uint32_t player_number);
	void create_first_settlement(const uint32_t player_number, const uint32_t player_turn);
	void create_first_edge(const uint32_t player_number, const uint32_t player_turn);
	void create_second_settlement(const uint32_t player_number, const uint32_t player_turn);
	std::unordered_map<CatanUtils::ResourceType, uint32_t> get_settlement_resources(
		const std::string& position) const;
	void create_second_edge(const uint32_t player_number, const uint32_t player_turn);

	// function for catan commands
	CatanUtils::ServerInfo handle_command(const uint32_t player_number, const std::string& data);
	CatanUtils::ServerInfo handle_command_when_not_your_turn(
		const uint32_t player_number, const std::string& data);
	CatanUtils::ServerInfo rob_resources_from_player(const uint32_t player_number,
	                                                 const std::string& data);
	CatanUtils::ServerInfo handle_create_edge(const uint32_t player_number,
	                                          const std::vector<std::string>& data);
	CatanUtils::ServerInfo handle_create_settlement(const uint32_t player_number,
	                                                const std::vector<std::string>& data);
	CatanUtils::ServerInfo handle_upgrade_settlement_to_city(const uint32_t player_number,
	                                                         const std::vector<std::string>& data);
	CatanUtils::ServerInfo handle_roll_dices(const uint32_t player_number,
	                                         const std::vector<std::string>& data);
	CatanUtils::ServerInfo handle_robber(const uint32_t player_number,
	                                     const std::vector<std::string>& data);
	void move_knight(const uint32_t player_number);
	void rob_with_knight(const uint32_t player_number);

	// functions for help
	bool is_possible_to_create_settlement(const CatanUtils::PlayerType player, const uint32_t row_number,
	                                      const uint32_t col_number) const;
	bool is_possible_to_create_edge(const CatanUtils::PlayerType player, const uint32_t row_number,
	                                const uint32_t col_number) const;
	void send_board_to_everyone() const;
	void send_player_resources(const uint32_t player_number) const;
	void pass_turn();
	const CatanUtils::BasicBoard& get_board() const;
	int32_t get_winner() const;

private:
	Server m_server;
	std::vector<std::shared_ptr<CatanUtils::Player>> m_players;
	std::vector<CatanUtils::WinUtils::Event> m_events;
	CatanUtils::BasicBoard m_board;
	bool m_game_started;
	uint32_t m_turn_number;
	bool m_is_robbed_on;
	// TODO: check if we already roll the dice, otherwise we can't pass the turn
	//bool m_rolled_dices;
};
