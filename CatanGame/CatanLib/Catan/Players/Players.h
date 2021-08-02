#pragma once

#include <unordered_map>


#include "Catan/CatanConsts.h"

namespace CatanUtils {

	class Player final
	{
	public:
		explicit Player(const PlayerType player_type);

		PlayerType get_player_type() const;
		uint32_t get_number_of_points() const;
		uint32_t get_number_of_specific_resource_card(const ResourceType resource) const;
		uint32_t get_number_of_available_resources() const;
		uint32_t get_number_of_development_cards() const;
		uint32_t get_number_of_knights() const;

		void add_to_specific_resource_card(const ResourceType resource_card, uint32_t number_of_cards);
		void remove_from_specific_resource_card(const ResourceType resource_card, uint32_t number_of_cards);

		void increase_development_card(const DevelopmentCards development_card);
		void decrease_development_card(const DevelopmentCards development_card);

		void combine_resources(const std::unordered_map<ResourceType, uint32_t>& resources);
		void resuce_resources(const std::unordered_map<ResourceType, uint32_t>& resources);
		
		void increase_number_of_knights();

		void increase_number_of_points(uint32_t number_of_points = 1);

	private:
		PlayerType m_player_type;
		std::unordered_map<DevelopmentCards, uint32_t> m_development_cards;
		std::unordered_map<ResourceType, uint32_t> m_resource_cards;
		uint32_t m_number_of_points;
		uint32_t m_number_of_knights;
	};

}
