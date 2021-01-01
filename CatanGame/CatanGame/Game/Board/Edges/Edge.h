#pragma once

#include <memory>
#include <utility>

#include "Game/Players/Players.h"

class Node;

class Edge final
{
public:
	explicit Edge(std::shared_ptr<Node> first_node, std::shared_ptr<Node> second_node,
	              const PlayerType player);
	PlayerType get_player() const;
	std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>& get_edge();
	std::shared_ptr<Node>& get_first_node();
	std::shared_ptr<Node>& get_second_node();


private:
	std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> m_edge;
	PlayerType m_player;
};
