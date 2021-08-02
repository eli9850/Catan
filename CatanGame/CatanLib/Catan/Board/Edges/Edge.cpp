#include "Edge.h"

namespace CatanUtils {

	Edge::Edge(const PlayerType player) :
		m_player(player)
	{
	}

	PlayerType Edge::get_player() const
	{
		return m_player;
	}

}
