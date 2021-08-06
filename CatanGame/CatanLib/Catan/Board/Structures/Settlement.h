#pragma once

#include "IStructure.h"

namespace CatanUtils
{
	class Settlement : public IStructure
	{
	public:
		explicit Settlement(const PlayerType player, std::vector<std::shared_ptr<Resource>> resources);
		std::unordered_map<ResourceType, uint32_t>
		get_resources(const uint32_t dice_number) const override;
	};
}
