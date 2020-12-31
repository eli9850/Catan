#include "Edge.h"

Edge::Edge(const PlayerType player): m_player(player)
{
}

PlayerType Edge::get_player() const
{
	return m_player;
}
