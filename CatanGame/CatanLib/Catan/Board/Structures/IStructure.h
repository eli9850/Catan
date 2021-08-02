#pragma once

#include "Catan/CatanConsts.h"
#include "Catan/Board/Resources/Resource.h"
#include <unordered_map>
#include <memory>

namespace CatanUtils {

	class IStructure {

	public:
		IStructure(PlayerType player, std::vector<std::shared_ptr<Resource>> resources) :
			m_player(player),
			m_resources(std::move(resources)) {}
		const PlayerType& get_player() const { return m_player; }
		virtual std::unordered_map<ResourceType, uint32_t> get_resources(const uint32_t dice_nubmer) const = 0;

	protected:
		PlayerType m_player;
		std::vector<std::shared_ptr<Resource>> m_resources;

	};
}
