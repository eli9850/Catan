#include "Board.h"

#include <algorithm>
#include <random>
#include <sstream>
#include <chrono>

Board::Board(): m_robber_resource_number(-1)
{
	create_random_resources();
	create_nodes();
}

void Board::create_nodes()
{
	for (auto& node : m_nodes)
	{
		node = std::make_shared<Node>();
	}
}

void Board::create_random_resources()
{
	std::array<uint32_t, 18> resource_numbers = {
		2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12
	};
	std::array<uint32_t, 18> resource_types = {
		1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5
	};
	const auto seed = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
	srand(seed);
	shuffle(resource_numbers.begin(), resource_numbers.end(), std::default_random_engine(seed));
	shuffle(resource_types.begin(), resource_types.end(), std::default_random_engine(seed));
	const auto empty = std::rand() % 19;
	m_resources[empty] = std::make_shared<Resource>(ResourceType::NONE, 7);
	for (auto i = 0; i < empty; i++)
	{
		m_resources[i] = std::make_shared<Resource>(static_cast<ResourceType>(resource_types[i]),
		                                            resource_numbers[i]);
	}
	for (auto i = empty + 1; i < NUMBER_OF_RESOURCES_IN_BOARD; i++)
	{
		m_resources[i] = std::make_shared<Resource>(
			static_cast<ResourceType>(resource_types[i - 1]), resource_numbers[i - 1]);
	}
}

std::string Board::to_string() const
{
	std::stringstream to_string;
	for (const auto& edge : m_edges)
	{
		to_string << std::to_string(static_cast<uint32_t>(edge->get_player()));
	}
	for (const auto& resource : m_resources)
	{
		to_string << resource->get_resource_number() << std::to_string(
			static_cast<uint32_t>(resource->get_resource_type()));
	}
	for (const auto& node : m_nodes)
	{
		to_string << static_cast<uint32_t>(node->get_settlement()->get_settlement_type()) <<
			static_cast<uint32_t>(node->get_settlement()->get_player_type());
	}
	return to_string.str();
}

std::shared_ptr<Node>& Board::get_node(const uint32_t index)
{
	return m_nodes[index];
}

std::shared_ptr<Edge>& Board::get_edge(const uint32_t index)
{
	return m_edges[index];
}

void Board::set_robber_number(const uint32_t resource_number)
{
	m_robber_resource_number = resource_number;
}

void Board::set_node_settlement(const uint32_t node_index, std::unique_ptr<Settlement> settlement)
{
	m_nodes[node_index]->set_settlement(std::move(settlement));
}

void Board::upgrade_node_settlement(const uint32_t node_index)
{
	m_nodes[node_index]->get_settlement()->upgrade_settlement();
}

void Board::create_edge(const uint32_t first_node_index, const uint32_t second_node_index,
                        const PlayerType player_type)
{
	// need to check if the edge is valid
	auto new_edge = std::make_shared<Edge>(m_nodes[first_node_index], m_nodes[second_node_index],
	                                       player_type);
	m_edges.emplace_back(std::move(new_edge));
}
