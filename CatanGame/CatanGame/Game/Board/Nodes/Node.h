#pragma once

#include <memory>

#include "Game/Board/Edges/Edge.h"
#include "Game/Board/Structures/Settlement.h"

class Node final
{
public:
	explicit Node();
	void set_settlement(std::unique_ptr<Settlement> settlement);
	
	std::shared_ptr<Edge>& get_first_edge();
	std::shared_ptr<Edge>& get_second_edge();
	std::shared_ptr<Edge>& get_third_edge();
	std::unique_ptr<Settlement>& get_settlement();

private:
	std::shared_ptr<Edge> m_first_edge;
	std::shared_ptr<Edge> m_second_edge;
	std::shared_ptr<Edge> m_third_edge;

	std::unique_ptr<Settlement> m_settlement;
};
