#pragma once

#include "IStructure.h"
#include "Game/Players/Players.h"

class Settlement : public IStructure
{
public:
	explicit Settlement(PlayerType player, std::vector<std::shared_ptr<Resource>> resources);
	std::unordered_map<ResourceType, uint8_t>  get_resources(const uint8_t dice_nubmer) override;
};