#pragma once

#include <cstdint>

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

	enum class CommandType
	{
		FINISH_TURN,
		SETTLEMENT,
		CITY,
		EDGE,
		ROLL_DICES,
		ROBBED_RESOURCES,
		MOVE_KNIGHT,
	};

	enum class CommandResult
	{
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
}
