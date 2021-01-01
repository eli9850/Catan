#include "Node.h"

Node::Node(): m_first_edge(nullptr), m_second_edge(nullptr), m_third_edge(nullptr),
              m_settlement(nullptr)
{
}

void Node::set_settlement(std::unique_ptr<Settlement> settlement)
{
	m_settlement = std::move(settlement);
}

std::shared_ptr<Edge>& Node::get_first_edge()
{
	return m_first_edge;
}

std::shared_ptr<Edge>& Node::get_second_edge()
{
	return m_second_edge;
}

std::shared_ptr<Edge>& Node::get_third_edge()
{
	return m_third_edge;
}

std::unique_ptr<Settlement>& Node::get_settlement()
{
	return m_settlement;
}
