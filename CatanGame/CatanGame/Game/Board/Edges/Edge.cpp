#include "Edge.h"

Edge::Edge(std::shared_ptr<Node> first_node, std::shared_ptr<Node> second_node, const PlayerType player):
	m_first_node(std::move(first_node)),
	m_second_node(std::move_if_noexcept(second_node)),
	m_player(player)
{
}

std::shared_ptr<Node>& Edge::get_first_node()
{
	return m_first_node;
}

std::shared_ptr<Node>& Edge::get_second_node()
{
	return m_second_node;
}



PlayerType Edge::get_player() const
{
	return m_player;
}
