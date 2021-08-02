#pragma once

namespace CatanUtils {

	enum class StructureType {
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
}
