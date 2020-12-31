#include "Node.h"

Node::Node(std::shared_ptr<Resource> first_resource, std::shared_ptr<Resource> second_resource,
           std::shared_ptr<Resource> third_resource, std::shared_ptr<Edge> first_edge,
           std::shared_ptr<Edge> second_edge, std::shared_ptr<Edge> third_edge):
	m_first_resource(std::move(first_resource)), m_second_resource(std::move(second_resource)),
	m_third_resource(std::move(third_resource)), m_first_edge(std::move(first_edge)),
	m_second_edge(std::move(second_edge)), m_third_edge(std::move(third_edge)),
	m_settlement(std::make_unique<Settlement>(SettlementType::NONE, PlayerType::NONE))
{
}

void Node::set_settlement(std::unique_ptr<Settlement> settlement)
{
	m_settlement = std::move(settlement);
}

PlayerType Node::get_player_type() const
{
	return m_settlement->get_player_type();
}

SettlementType Node::get_settlement_type() const
{
	return m_settlement->get_settlement_type();
}
