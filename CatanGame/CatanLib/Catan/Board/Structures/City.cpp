#include "City.h"

namespace CatanUtils
{
	City::City(const PlayerType player, std::vector<std::shared_ptr<Resource>> resources) : IStructure(
		player, std::move(resources))
	{
	}

	std::unordered_map<ResourceType, uint32_t> City::get_resources(const uint32_t dice_number) const
	{
		std::unordered_map<ResourceType, uint32_t> result;

		for (const auto& resource : m_resources)
		{
			if (resource->get_resource_number() == dice_number && !resource->
				is_robber_on())
			{
				if (result.find(resource->get_resource_type()) == result.end())
				{
					result.try_emplace(resource->get_resource_type(), 2);
				}
				else
				{
					result.at(resource->get_resource_type()) = 2;
				}
			}
		}
		return result;
	}
}
