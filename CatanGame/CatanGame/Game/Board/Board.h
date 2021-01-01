#pragma once

#include <vector>
#include <array>
#include <memory>
#include <string>

#include "Utils/Consts.h"

#include "Nodes/Node.h"
#include "Edges/Edge.h"
#include "Resources/Resource.h"


class Board
{
public:
	explicit Board();
	std::string to_string() const;

	void set_node_settlement(const uint32_t node_index, std::unique_ptr<Settlement> settlement);
	void upgrade_node_settlement(const uint32_t node_index);
	void create_edge(const uint32_t first_node_index, const uint32_t second_node_index,
	                 const PlayerType player_type);
	void set_robber_number(const uint32_t resource_number);


	// This private section contains private methods that initialize the board
private:
	void create_random_resources();
	void create_nodes();

	// I don't know if I need those functions
	std::shared_ptr<Node>& get_node(const uint32_t index);
	std::shared_ptr<Edge>& get_edge(const uint32_t index);

	// This private section is for member of the board class
private:
	uint32_t m_robber_resource_number;
	std::array<std::shared_ptr<Node>, NUMBER_OF_NODES_IN_BOARD> m_nodes;
	std::vector<std::shared_ptr<Edge>> m_edges;
	std::array<std::shared_ptr<Resource>, NUMBER_OF_RESOURCES_IN_BOARD> m_resources;
};
