#pragma once

#include "Game/Players/Players.h"

class Edge final
{
public:
	explicit Edge(const PlayerType player);
	PlayerType get_player() const;

private:
	PlayerType m_player;
};
