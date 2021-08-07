#include "Players.h"
#include "Exceptions/PlayerExceptions.h"
#include "Exceptions/MapExceptions.h"
#include "Utils/MapUtils.h"

#include <sstream>

#include "Utils/StringUtils.h"
#include "Utils/RandomUtils.h"

namespace CatanUtils
{
	Player::Player(const PlayerType player_type) : m_player_type(player_type), m_number_of_points(0),
	                                               m_number_of_knights(0)
	{
		m_development_cards.try_emplace(DevelopmentCards::ABUNDANCE_CARD, 0);
		m_development_cards.try_emplace(DevelopmentCards::ROAD_CARD, 0);
		m_development_cards.try_emplace(DevelopmentCards::POINT_CARD, 0);
		m_development_cards.try_emplace(DevelopmentCards::KNIGHT_CARD, 0);
		m_development_cards.try_emplace(DevelopmentCards::MONOPOL_CARD, 0);

		m_resource_cards.try_emplace(ResourceType::CLAY, 0);
		m_resource_cards.try_emplace(ResourceType::WHEAT, 0);
		m_resource_cards.try_emplace(ResourceType::TREE, 0);
		m_resource_cards.try_emplace(ResourceType::STONE, 0);
		m_resource_cards.try_emplace(ResourceType::SHEEP, 0);
	}

	const std::unordered_map<DevelopmentCards, uint32_t>& Player::get_development_cards() const
	{
		return m_development_cards;
	}

	const std::unordered_map<ResourceType, uint32_t>& Player::get_resource_cards() const
	{
		return m_resource_cards;
	}

	std::string Player::get_resources_str()
	{
		std::stringstream resources;
		for (const auto& [key, value] : m_resource_cards)
		{
			resources << std::to_string(static_cast<uint32_t>(key)) << "," << std::to_string(value) <<
				";";
		}
		return resources.str();
	}

	void Player::update_resources(const std::string& data)
	{
		for (const auto& resource_str : StringUtils::split(data, ";"))
		{
			const auto element_data = StringUtils::split(resource_str, ",");
			const auto key = static_cast<CatanUtils::ResourceType>(std::stoi(element_data.at(0)));
			const auto value = std::stoi(element_data.at(1));
			m_resource_cards.at(key) = value;
		}
	}

	std::string Player::get_development_cards_str()
	{
		std::stringstream developments_cards;
		for (const auto& [key, value] : m_development_cards)
		{
			developments_cards << std::to_string(static_cast<uint32_t>(key)) << "," << std::to_string(value) <<
				";";
		}
		return developments_cards.str();
	}

	void Player::update_development_cards(const std::string& data)
	{
		for (const auto& development_card_str : StringUtils::split(data, ";"))
		{
			const auto element_data = StringUtils::split(development_card_str, ",");
			const auto key = static_cast<CatanUtils::DevelopmentCards>(std::stoi(element_data.at(0)));
			const auto value = std::stoi(element_data.at(1));
			m_development_cards.at(key) = value;
		}
	}

	PlayerType Player::get_player_type() const
	{
		return m_player_type;
	}

	uint32_t Player::get_number_of_points() const
	{
		return m_number_of_points;
	}

	uint32_t Player::get_number_of_specific_resource_card(const ResourceType resource) const
	{
		return m_resource_cards.at(resource);
	}

	uint32_t Player::get_number_of_available_resources() const
	{
		return MapUtils::get_sum_of_values(m_resource_cards);
	}

	uint32_t Player::get_number_of_specific_development_card(const DevelopmentCards development_card) const
	{
		return m_development_cards.at(development_card);
	}
	
	uint32_t Player::get_number_of_development_cards() const
	{
		return MapUtils::get_sum_of_values(m_development_cards);
	}

	uint32_t Player::get_number_of_knights() const
	{
		return m_number_of_knights;
	}

	void Player::add_to_specific_resource_card(const ResourceType resource_card,
	                                           const uint32_t number_of_cards)
	{
		m_resource_cards.at(resource_card) += number_of_cards;
	}

	void Player::remove_from_specific_resource_card(const ResourceType resource_card,
	                                                const uint32_t number_of_cards)
	{
		m_resource_cards.at(resource_card) -= number_of_cards;
	}

	void Player::increase_development_card(const DevelopmentCards development_card)
	{
		m_development_cards.at(development_card)++;
	}

	void Player::decrease_development_card(const DevelopmentCards development_card)
	{
		m_development_cards.at(development_card)--;
	}

	void Player::combine_resources(const std::unordered_map<ResourceType, uint32_t>& resources)
	{
		m_resource_cards = MapUtils::get_combine_maps(m_resource_cards, resources);
	}

	void Player::reduce_resources(const std::unordered_map<ResourceType, uint32_t>& resources)
	{
		try
		{
			m_resource_cards = MapUtils::get_slashed_maps(m_resource_cards, resources);
		}
		catch (const MapException&)
		{
			throw WrongNumberOfSlashResourceException(
				"You were trying to slash more resources then you have");
		}
	}

	void Player::increase_number_of_knights()
	{
		m_number_of_knights++;
	}

	void Player::increase_number_of_points(const uint32_t number_of_points)
	{
		m_number_of_points += number_of_points;
	}

	ResourceType Player::get_random_resource() const
	{
		std::vector<ResourceType> all_resources;
		for (const auto& [resource_type, number_of_resources] : m_resource_cards)
		{
			for (size_t i = 0; i < number_of_resources; i++)
			{
				all_resources.emplace_back(resource_type);
			}
		}
		const auto random_index = CatanUtils::RandomUtils::get_random_number(
			0, get_number_of_available_resources());

		return all_resources.at(random_index);
	}
}
