#pragma once

#include "Game/Players/Players.h"

class Node;

class Edge final
{
public:
	explicit Edge(const PlayerType player);
	PlayerType get_player() const;
	void set_player(const PlayerType player);


private:
	PlayerType m_player;
};
