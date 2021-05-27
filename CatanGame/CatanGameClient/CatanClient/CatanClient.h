#pragma once

#include "Client/Client.h"
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

enum class CommandType {
	FINISH_TURN,
	SETTLEMENT,
	CITY,
	EDGE,
};

enum class CommandResult {
	SUCCESS,
	INFO,
	YOUR_TURN,
	NOT_YOUR_TURN,
	ONLY_SETTLEMENT,
	ONLY_EDGE,
	NOT_ENOUGH_RESOURCES,
	INVALID_PLACE,
	TURN_AS_FINISHED,
};

class CatanClient
{
public:
	CatanClient(const std::string& ip, const std::string& potr_number);
	void start_game();

private:
	void recive_from_server();
	void handle_build_settlement();
	void handle_upgrade_settlement_to_city();
	void handle_build_edge();
	void handle_finish_turn();

private:
	std::queue<std::string> m_command_result;
	std::unordered_map<DevelopmentCards, uint8_t> m_development_cards;
	std::unordered_map<ResourceType, uint8_t> m_resource_cards;
	uint32_t m_number_of_points;
	Client m_client;
	bool m_game_is_finished;
};

