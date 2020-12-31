#pragma once

#include "../../Players/Players.h"

class Edge final
{
public:
	explicit Edge(PlayerType player);
	PlayerType get_player() const;

private:
	PlayerType m_player;
};

