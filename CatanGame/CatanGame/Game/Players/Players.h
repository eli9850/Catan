#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "Game/Board/Structures/Settlement.h"

enum class ResourceCards
{
	WHEAT_CARD,
	CLAY_CARD,
	SHEEP_CARD,
	TREE_CARD,
	STONE_CARD,
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
	RED,
	YELLOW,
	WHITE,
};

class Player final
{
public:
	explicit Player(const PlayerType player_type);
	void increase_resource_card(const ResourceCards resource_card);
	void increase_development_card(const DevelopmentCards development_card);
	void decrease_resource_card(const ResourceCards resource_card);
	void decrease_development_card(const DevelopmentCards development_card);
	void add_settlement(std::shared_ptr<Settlement> settlement);
	PlayerType get_player_type() const;
	uint32_t get_number_of_points() const;


private:
	PlayerType m_player_type;
	std::vector<std::shared_ptr<Settlement>> m_settlements;
	std::unordered_map<DevelopmentCards, uint32_t> m_development_cards;
	std::unordered_map<ResourceCards, uint32_t> m_resource_cards;
	uint32_t m_number_of_points;
};
