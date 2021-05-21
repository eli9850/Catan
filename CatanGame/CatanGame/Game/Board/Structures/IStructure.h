#pragma once

#include "Game/Board/Resources/Resource.h"
#include "Game/Players/Players.h"

class IStructure {

public:
	IStructure(PlayerType player) :m_player(player){}
	virtual void get_resources(const Resource resource) = 0;
	const Player& get_player() const { return m_player; }

protected:
	Player m_player;
	
};