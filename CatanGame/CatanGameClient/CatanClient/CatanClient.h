#pragma once

#include "GUI/GUIClient.h"
#include "Client/Client.h"
#include "WaitQueue/WaitQueue.h"
#include "WinWrapers/Events/Event.h"

#include <string>
#include <unordered_map>

#include "Catan/CatanConsts.h"
#include "Catan/Players/Players.h"


class CatanClient
{
public:
	explicit CatanClient(const std::string& ip, const std::string& port_number);
	void start_game();

private:
	void get_commands_from_server();
	void handle_server_commands();
	void handle_player();
	void handle_buy_development_card();
	void handle_use_development_card();
	void handle_road_building_card();
	void handle_monopoly_card();
	void handle_abundance_card();
	//void handle_knight_card();
	void handle_build_settlement();
	void handle_upgrade_settlement_to_city();
	void handle_build_edge();
	void handle_finish_turn();
	void handle_roll_dices();
	void handle_rob_resources();
	void handle_move_knight();
	void handle_choose_player_to_rob();
	void update_new_resources_info(const std::string& data);
	void update_new_development_cards_info(const std::string& data);
	void update_dices(const std::string& data);
	std::string choose_resources_to_rob() const;

private:
	std::unique_ptr<CatanUtils::Player> m_player;
	CatanUtils::QueueUtils::WaitQueue m_commands;
	CatanUtils::QueueUtils::WaitQueue m_command_result;
	Client m_client;
	GUIClient m_gui_client;
	bool m_game_is_finished;
	bool m_is_game_start;
	CatanUtils::WinUtils::Event m_is_my_turn;
};

std::string convert_resource_type_to_string(const CatanUtils::ResourceType resource);
