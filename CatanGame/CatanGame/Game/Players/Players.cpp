#include "Players.h"

Player::Player(const PlayerType player_type) : m_player_type(player_type), m_settlements(),
                                               m_number_of_points(0)
{
	m_development_cards[DevelopmentCards::ABUNDANCE_CARD] = 0;
	m_development_cards[DevelopmentCards::ROAD_CARD] = 0;
	m_development_cards[DevelopmentCards::POINT_CARD] = 0;
	m_development_cards[DevelopmentCards::KNIGHT_CARD] = 0;

	m_resource_cards[ResourceCards::CLAY_CARD] = 0;
	m_resource_cards[ResourceCards::WHEAT_CARD] = 0;
	m_resource_cards[ResourceCards::TREE_CARD] = 0;
	m_resource_cards[ResourceCards::STONE_CARD] = 0;
	m_resource_cards[ResourceCards::SHEEP_CARD] = 0;
}

void Player::increase_development_card(const DevelopmentCards development_card)
{
	m_development_cards[development_card]++;
}

void Player::increase_resource_card(const ResourceCards resource_card)
{
	m_resource_cards[resource_card]++;
}

void Player::decrease_development_card(const DevelopmentCards development_card)
{
	m_development_cards[development_card]--;
}

void Player::decrease_resource_card(const ResourceCards resource_card)
{
	m_resource_cards[resource_card]--;
}

void Player::add_settlement(std::shared_ptr<Settlement> settlement)
{
	m_settlements.emplace_back(std::move(settlement));
}

uint32_t Player::get_number_of_points() const
{
	return m_number_of_points;
}

PlayerType Player::get_player_type() const
{
	return m_player_type;
}


