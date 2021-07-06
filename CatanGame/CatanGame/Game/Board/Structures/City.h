#pragma once

#include "IStructure.h"
#include "Game/Players/Players.h"

class City : public IStructure
{
public:
	explicit City(const PlayerType player, std::vector<std::shared_ptr<Resource>> resources);
	std::unordered_map<ResourceType, uint8_t>  get_resources(const uint8_t dice_nubmer) override;
};