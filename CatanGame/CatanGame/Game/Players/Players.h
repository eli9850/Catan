#pragma once

#include <unordered_map>
#include "Game/Board/Resources/Resource.h"


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

class Player final
{
public:
	explicit Player(const PlayerType player_type);
	void increase_resource_card(const ResourceType resource_card);
	void increase_development_card(const DevelopmentCards development_card);
	void decrease_resource_card(const ResourceType resource_card);
	void decrease_development_card(const DevelopmentCards development_card);
	void combine_resources(const std::unordered_map<ResourceType, uint8_t>& resources);
	PlayerType get_player_type() const;
	uint32_t get_number_of_points() const;
	uint8_t get_number_of_resource_cards(const ResourceType resource) const;


private:
	PlayerType m_player_type;
	std::unordered_map<DevelopmentCards, uint8_t> m_development_cards;
	std::unordered_map<ResourceType, uint8_t> m_resource_cards;
	uint32_t m_number_of_points;
};
