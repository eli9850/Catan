#pragma once

#include "IStructure.h"
#include "Game/Players/Players.h"

class Settlement : public IStructure
{
public:
	explicit Settlement(PlayerType player);
	void get_resources(const Resource resource) override;
};