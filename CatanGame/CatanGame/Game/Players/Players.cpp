#include "Players.h"
#include "Utils/MapUtils.h"

Player::Player(const PlayerType player_type) : m_player_type(player_type), m_number_of_points(0)
{
	m_development_cards[DevelopmentCards::ABUNDANCE_CARD] = 0;
	m_development_cards[DevelopmentCards::ROAD_CARD] = 0;
	m_development_cards[DevelopmentCards::POINT_CARD] = 0;
	m_development_cards[DevelopmentCards::KNIGHT_CARD] = 0;

	m_resource_cards[ResourceType::CLAY] = 0;
	m_resource_cards[ResourceType::WHEAT] = 0;
	m_resource_cards[ResourceType::TREE] = 0;
	m_resource_cards[ResourceType::STONE] = 0;
	m_resource_cards[ResourceType::SHEEP] = 0;
}

void Player::combine_resources(const std::unordered_map<ResourceType, uint8_t>& resources) {
	m_resource_cards = get_combine_maps(m_resource_cards, resources);
}

void Player::increase_development_card(const DevelopmentCards development_card)
{
	m_development_cards[development_card]++;
}

void Player::increase_resource_card(const ResourceType resource_card)
{
	m_resource_cards[resource_card]++;
}

void Player::decrease_development_card(const DevelopmentCards development_card)
{
	m_development_cards[development_card]--;
}

void Player::decrease_resource_card(const ResourceType resource_card)
{
	m_resource_cards[resource_card]--;
}

uint32_t Player::get_number_of_points() const
{
	return m_number_of_points;
}

PlayerType Player::get_player_type() const
{
	return m_player_type;
}

uint8_t Player::get_number_of_resource_cards(const ResourceType resource) const {
	return m_resource_cards.at(resource);
}