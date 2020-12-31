#pragma once

#include <memory>

#include "../Resources/Resource.h"
#include "../Edges/Edge.h"
#include "../Structures/Settlement.h"

class Node final
{
public:
	explicit Node(std::shared_ptr<Resource> first_resource, std::shared_ptr<Resource> second_resource, std::shared_ptr<Resource> third_resource, std::shared_ptr<Edge> first_edge, std::shared_ptr<Edge> second_edge, std::shared_ptr<Edge> third_edge);
	void set_settlement(std::unique_ptr<Settlement> settlement);

	SettlementType get_settlement_type() const;
	PlayerType get_player_type() const;

private:
	std::shared_ptr<Resource> m_first_resource;
	std::shared_ptr<Resource> m_second_resource;
	std::shared_ptr<Resource> m_third_resource;

	std::shared_ptr<Edge> m_first_edge;
	std::shared_ptr<Edge> m_second_edge;
	std::shared_ptr<Edge> m_third_edge;

	std::unique_ptr<Settlement> m_settlement;
};

