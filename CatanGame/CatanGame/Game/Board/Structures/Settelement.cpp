#include "Settlement.h"

Settlement::Settlement(PlayerType player): IStructure(player) {}

void Settlement::get_resources(const Resource resource) {
	m_player.increase_resource_card(resource.get_resource_type());
}