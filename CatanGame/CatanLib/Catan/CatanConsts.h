#pragma once

#include <cstdint>
#include <string_view>

namespace CatanUtils
{
	namespace Consts
	{
		constexpr uint32_t MIN_RESOURCE_NUMBER = 2;
		constexpr uint32_t MAX_RESOURCE_NUMBER = 12;
		constexpr uint32_t NUMBER_OF_RESOURCES_IN_ROW = 5;
		constexpr uint32_t NUMBER_OF_RESOURCES_IN_COLUMN = 5;
		constexpr uint32_t NUMBER_OF_NODES_IN_ROW = 12;
		constexpr uint32_t NUMBER_OF_NODES_IN_COLUMN = 6;
		constexpr uint32_t NUMBER_OF_EDGES_IN_ROW = 11;
		constexpr uint32_t NUMBER_OF_EDGES_IN_COLUMN = 11;
		constexpr uint32_t NUMBER_OF_VALID_NODES = 19;
		constexpr uint32_t NUMBER_OF_DESERT_RESOURCES = 1;
		constexpr uint32_t NUMBER_OF_DESERT_RESOURCE = 0;
		constexpr uint32_t NON_EXISTS_PLAYER_NUMBER = 5;
		constexpr uint32_t EMPTY_NODE = 0;
		constexpr std::string_view COMMAND_END_MAGIC = "ABCD1234";
	}

	enum class StructureType
	{
		NONE,
		SETTLEMENT,
		CITY
	};

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

	enum class ClientCommands
	{
		CREATE_SETTLEMENT,
		CREATE_CITY,
		CREATE_EDGE,

		ROLL_DICES,
		FINISH_TURN,

		ROB_RESOURCES,

		MOVE_KNIGHT,

		EXIT_GAME,
	};

	enum class ServerInfo
	{
		CONNECT_INFO,
		BOARD_INFO,
		YOUR_TURN,
		NOT_YOUR_TURN,
		ONLY_SETTLEMENT,
		ONLY_EDGE,
		CREATE_SETTLEMENT_SUCCEEDED,
		CREATE_EDGE_SUCCEEDED,
		NEW_RESOURCES,
		FINISH_TURN_SUCCEEDED,
		CREATE_CITY_SUCCEEDED,
		INVALID_COMMAND,
		ROB_RESOURCES_SUCCEEDED,
		WRONG_NUMBER_OF_RESOURCES_TO_ROB,
		INVALID_EDGE_PLACE,
		INVALID_STRUCTURE_PLACE,
		NOT_ENOUGH_RESOURCES,
		DICES_NUMBERS,
		ROLL_DICES_SUCCEEDED,
		ROBBER,
		ONLY_ROBBER,
		KNIGHT,
		ONLY_KNIGHT,
		MOVE_KNIGHT_SUCCEEDED,
		INVALID_RESOURCE_PLACE,
	};
}
