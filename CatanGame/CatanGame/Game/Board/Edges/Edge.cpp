#include "Edge.h"

Edge::Edge(std::shared_ptr<Node> first_node, std::shared_ptr<Node> second_node, const PlayerType player) :
	m_edge(std::move(first_node), std::move(second_node)),
	m_player(player)
{
}

std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>& Edge::get_edge()
{
	return m_edge;
}


std::shared_ptr<Node>& Edge::get_first_node()
{
	return m_edge.first;
}

std::shared_ptr<Node>& Edge::get_second_node()
{
	return m_edge.second;
}



PlayerType Edge::get_player() const
{
	return m_player;
}
