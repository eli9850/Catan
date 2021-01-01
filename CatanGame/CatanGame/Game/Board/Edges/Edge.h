#pragma once

#include <memory>

#include "Game/Players/Players.h"

class Node;

class Edge final
{
public:
	explicit Edge(std::shared_ptr<Node> first_node, std::shared_ptr<Node> second_node,
	              const PlayerType player);
	PlayerType get_player() const;
	std::shared_ptr<Node>& get_first_node();
	std::shared_ptr<Node>& get_second_node();


private:
	std::shared_ptr<Node> m_first_node;
	std::shared_ptr<Node> m_second_node;
	PlayerType m_player;
};
