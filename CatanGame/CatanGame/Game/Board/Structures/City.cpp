#include "City.h"

City::City(const PlayerType player): IStructure(player){}

void City::get_resources(const Resource resource) {
	m_player.increase_resource_card(resource.get_resource_type());
	m_player.increase_resource_card(resource.get_resource_type());
}