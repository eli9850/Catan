#pragma once

#include "GUI/GUIClient.h"
#include "Client/Client.h"
#include "WaitQueue/WaitQueue.h"
#include "Events/Event.h"

#include <string>
#include <unordered_map>
#include <queue>

enum class ResourceType
{
	NONE,
	WHEAT,
	CLAY,
	SHEEP,
	TREE,
	STONE
};

enum class DevelopmentCards
{
	KNIGHT_CARD,
	ABUNDANCE_CARD,
	ROAD_CARD,
	POINT_CARD,
	MONOPOL_CARD,
};

enum class PlayerType
{
	BLUE,
	YELLOW,
	GREEN,
	RED,
};

enum class StructureType {
	NONE,
	SETTLEMENT,
	CITY
};

enum class CommandType {
	FINISH_TURN,
	SETTLEMENT,
	CITY,
	EDGE,
	ROLL_DICES,
	ROBBED_RESOURCES,
	MOVE_KNIGHT,
};

enum class CommandResult {
	SUCCESS,
	INFO,
	DICES_NUMBERS,
	NEW_TURN_INFO,
	ROBBER,
	YOUR_TURN,
	NOT_YOUR_TURN,
	ONLY_SETTLEMENT,
	ONLY_EDGE,
	NOT_ENOUGH_RESOURCES,
	INVALID_PLACE,
	TURN_AS_FINISHED,
	FAILED_TO_ROB,
};

class CatanClient
{
public:
	CatanClient(const std::string& ip, const std::string& potr_number);
	void start_game();

private:
	void get_commands_from_server();
	void handle_server_commands();
	void handle_player();
	void handle_build_settlement();
	void handle_upgrade_settlement_to_city();
	void handle_build_edge();
	void handle_finish_turn();
	void handle_roll_dices();
	void handle_robbed_resources();
	void handle_move_knight();
	void update_new_turn_info(const std::string& data);
	std::string choose_resources_to_rob() const;
	void rob_recources(const std::string& resources_to_robbed);

private:
	WaitQueue m_commands;
	std::queue<std::string> m_command_result;
	std::unordered_map<DevelopmentCards, uint8_t> m_development_cards;
	std::unordered_map<ResourceType, uint32_t> m_resource_cards;
	uint32_t m_number_of_points;
	Client m_client;
	GUIClient m_gui_client;
	bool m_game_is_finished;
	Event m_is_my_turn;

};

std::string convert_resource_type_to_string(ResourceType resource);
