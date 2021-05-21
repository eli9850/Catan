#pragma once

#include "IStructure.h"
#include "Game/Players/Players.h"

class City : public IStructure
{
public:
	explicit City(const PlayerType player);
	void get_resources(const Resource resource) override;
};