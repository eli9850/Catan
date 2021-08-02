#include "Players.h"
#include "Utils/StringUtils.h"
#include "Utils/MapUtils.h"
#include "Exceptions/PlayerExceptions.h"

Player::Player(const PlayerType player_type) : m_player_type(player_type), m_number_of_points(0)
{
	m_development_cards.try_emplace(DevelopmentCards::ABUNDANCE_CARD, 0);
	m_development_cards.try_emplace(DevelopmentCards::ROAD_CARD, 0);
	m_development_cards.try_emplace(DevelopmentCards::POINT_CARD, 0);
	m_development_cards.try_emplace(DevelopmentCards::KNIGHT_CARD, 0);

	m_resource_cards.try_emplace(ResourceType::CLAY, 0);
	m_resource_cards.try_emplace(ResourceType::WHEAT, 0);
	m_resource_cards.try_emplace(ResourceType::TREE, 0);
	m_resource_cards.try_emplace(ResourceType::STONE, 0);
	m_resource_cards.try_emplace(ResourceType::SHEEP, 0);
}

void Player::combine_resources(const std::unordered_map<ResourceType, uint8_t>& resources) {
	m_resource_cards = get_combine_maps(m_resource_cards, resources);
}

void Player::increase_development_card(const DevelopmentCards development_card)
{
	m_development_cards.at(development_card)++;
}

void Player::increase_resource_card(const ResourceType resource_card)
{
	m_resource_cards.at(resource_card)++;
}

void Player::decrease_development_card(const DevelopmentCards development_card)
{
	m_development_cards.at(development_card)--;
}

void Player::decrease_resource_card(const ResourceType resource_card)
{
	m_resource_cards.at(resource_card)--;
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

uint8_t Player::get_number_of_available_resources() const {
	uint8_t number_of_available_resources = 0;
	for (const auto& resource : m_resource_cards) {
		number_of_available_resources += resource.second;
	}
	return number_of_available_resources;
}

void Player::rob_resources(const std::string& resources_to_robbed) {
	uint8_t number_of_avialable_resource = get_number_of_available_resources();
	uint8_t number_of_resources_to_rob = 0;

	for (auto& resource_str : split(resources_to_robbed, ";")) {
		auto value = stoi(split(resource_str, ",").at(1));
		number_of_resources_to_rob += value;
	}
	if (number_of_avialable_resource / 2 != number_of_resources_to_rob) {
		throw WrongNumberOfRobbedResourceException("Wrong number of resources to robbed");
	}

	for (auto& resource_str : split(resources_to_robbed, ";")) {
		auto key = static_cast<ResourceType>(atoi(split(resource_str, ",").at(0).c_str()));
		auto value = stoi(split(resource_str, ",").at(1));
		m_resource_cards.at(key) -= value;
	}
}