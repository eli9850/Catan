#pragma once

#include "Game/Board/Resources/Resource.h"
#include "Game/Players/Players.h"
#include <unordered_map>
#include <memory>

class IStructure {

public:
	IStructure(PlayerType player, std::vector<std::shared_ptr<Resource>> resources) :
		m_player(player), 
		m_resources(std::move(resources)){}
	const PlayerType& get_player() const { return m_player; }
	virtual std::unordered_map<ResourceType, uint8_t> get_resources(const uint32_t dice_nubmer) = 0;

protected:
	PlayerType m_player;
	std::vector<std::shared_ptr<Resource>> m_resources;
	
};