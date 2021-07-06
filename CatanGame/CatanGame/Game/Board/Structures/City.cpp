#include "City.h"

City::City(const PlayerType player, std::vector<std::shared_ptr<Resource>> resources): IStructure(player, std::move(resources)){}

std::unordered_map<ResourceType, uint8_t>  City::get_resources(const uint8_t dice_nubmer) {
	std::unordered_map<ResourceType, uint8_t> result;

	for (uint8_t i = 0; i < m_resources.size(); i++)
	{
		if (m_resources[i]->get_resource_number() == dice_nubmer) {
			if (result.find(m_resources[i]->get_resource_type()) == result.end()) {
				result[m_resources[i]->get_resource_type()] = 2;
			}
			else {
				result[m_resources[i]->get_resource_type()] += 2;
			}

		}
	}
	return result;
}