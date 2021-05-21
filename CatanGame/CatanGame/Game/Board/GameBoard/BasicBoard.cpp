#include "BasicBoard.h"

#include "Exceptions/BoardExceptions.h"
#include "Game/Players/Players.h"
#include "Game/Board/Structures/City.h"
#include "Game/Board/Structures/Settlement.h"

#include <chrono>
#include <algorithm>
#include <random>
#include <sstream>

std::string BasicBoard::to_string() const
{
	std::stringstream to_string;
	
	for (auto i = 0; i < NUMBER_OF_RESOURCES_IN_ROW; i++) {
		for (auto j = 0; j < NUMBER_OF_RESOURCES_IN_COLOMN; j++) {
			to_string << std::to_string(static_cast<uint8_t>(m_resources[i][j]->get_resource_type()))
				<< "," << m_resources[i][j]->get_resource_number() << ";";
		}
	}
	to_string << "\n";
	for (auto i = 0; i < NUMBER_OF_NODES_IN_ROW; i++) {
		for (auto j = 0; j < NUMBER_OF_NODES_IN_COLOMN; j++) {
			if (m_nodes[i][j]->get_structure() == nullptr) {
				to_string << "0,0;";
			}
			else {
				to_string << std::to_string(static_cast<uint8_t>(m_nodes[i][j]->get_structure_type())) << "," <<
					std::to_string(static_cast<uint8_t>(m_nodes[i][j]->get_structure()->get_player().get_player_type()))
					<< ";";
			}

		}
	}
	to_string << "\n";
	for (auto i = 0; i < NUMBER_OF_EDGES_IN_ROW; i++){
		for (auto j = 0; j < NUMBER_OF_EDGES_IN_COLOMN; j++) {
			if (m_edges[i][j] == nullptr) {
				to_string << "0;";
			}
			else {
				to_string << std::to_string(static_cast<uint8_t>(m_edges[i][j]->get_player())) << ";";
			}
		}
	}
	to_string << "\n" << std::to_string(m_robber_resource_number.first) << "," << std::to_string(m_robber_resource_number.second);

	return to_string.str();
}

void BasicBoard::create_board()
{
	std::array<uint32_t, NUMBER_OF_VALID_NODE - 1> resource_numbers = {
		2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12
	};
	std::array<ResourceType, NUMBER_OF_VALID_NODE> resource_types = {
		ResourceType::NONE,
		ResourceType::SHEEP,ResourceType::SHEEP,ResourceType::SHEEP,ResourceType::SHEEP,
		ResourceType::WHEAT,ResourceType::WHEAT,ResourceType::WHEAT,ResourceType::WHEAT,
		ResourceType::TREE,ResourceType::TREE,ResourceType::TREE,ResourceType::TREE,
		ResourceType::STONE,ResourceType::STONE,ResourceType::STONE,
		ResourceType::CLAY,ResourceType::CLAY,ResourceType::CLAY,
	};
	const auto seed = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
	srand(seed);
	shuffle(resource_numbers.begin(), resource_numbers.end(), std::default_random_engine(seed));
	shuffle(resource_types.begin(), resource_types.end(), std::default_random_engine(seed));
	uint32_t resource_type_index = 0;
	uint32_t resource_number_index = 0;
	for (int i = 0; i < NUMBER_OF_RESOURCES_IN_ROW; i++)
	{
		for (int j = 0; j < NUMBER_OF_RESOURCES_IN_COLOMN; j++)
		{
			if (is_valid_node_index(i, j)) {
				if (resource_types[resource_type_index] == ResourceType::NONE) {
					m_resources[i][j] = std::make_shared<Resource>(ResourceType::NONE, 0);
					m_resources[i][j]->set_is_robber_on(true);
					m_robber_resource_number.first = i;
					m_robber_resource_number.second= j;
					resource_type_index++;
					continue;
				}
				m_resources[i][j] = std::make_shared<Resource>(resource_types[resource_type_index], resource_numbers[resource_number_index]);
				resource_type_index++;
				resource_number_index++;
			}
			else {
				m_resources[i][j] = std::make_shared<Resource>(ResourceType::NONE, 0);
			}
		}
	}
	for (int i = 0; i < NUMBER_OF_NODES_IN_ROW; i++)
	{
		for (int j = 0; j < NUMBER_OF_NODES_IN_COLOMN; j++)
		{
			m_nodes[i][j] = std::make_shared<Node>();
		}
	}
}

void BasicBoard::set_robber_number(const std::pair<uint8_t, uint8_t> resource_number)
{
	if (!is_valid_node_index(resource_number.first, resource_number.second)) {
		throw InvalidNodeIndex("You are trying to put the robber in an invalid node");
	}
	m_resources[m_robber_resource_number.first][m_robber_resource_number.second].get()->set_is_robber_on(false);
	m_resources[resource_number.first][resource_number.second].get()->set_is_robber_on(true);
	m_robber_resource_number = resource_number;
}

void BasicBoard::create_edge(const uint8_t edge_row, const uint8_t edge_col, const PlayerType player_type)
{
	if (edge_row % 2 == 0 && edge_col == NUMBER_OF_EDGES_IN_COLOMN) {
		throw InvalidEdgeIndex("The index of the edge is invalid");
	}
	if (edge_row % 2 == 1 && edge_col % 2 == 1) {
		throw InvalidEdgeIndex("The index of the edge is invalid");
	}
	m_edges[edge_row][edge_col] = std::make_shared<Edge>(player_type);
}

void BasicBoard::create_settlement(const uint8_t node_row, const uint8_t node_col, const PlayerType player_type) {
	if (!is_valid_node_index(node_row, node_col)) {
		throw InvalidNodeIndex("The index of the node is invalid");
	}
	m_nodes[node_row][node_col]->set_structure(std::make_unique<Settlement>(player_type));
	m_nodes[node_row][node_col]->set_structure_type(StructureType::SETTLEMENT);
}

void BasicBoard::upgrade_settlement_to_city(const uint8_t node_row, const uint8_t node_col) {
	auto structure = m_nodes[node_row][node_col];
	if (structure->get_structure_type() != StructureType::SETTLEMENT) {
		throw NotSettelemnt("You are trying to create city without settlement");
	}
	m_nodes[node_row][node_col]->set_structure(std::make_unique<City>(structure->get_structure()->get_player().get_player_type()));
	m_nodes[node_row][node_col]->set_structure_type(StructureType::CITY);
	
}

bool BasicBoard::is_valid_node_index(const uint8_t node_row, const uint8_t node_col) const {
	if (node_row == 0 && (node_col == 0 || node_col == 1)) {
		return false;
	}
	if (node_row == 1 && node_col == 0) {
		return false;
	}
	if (node_row == 3 && node_col == NUMBER_OF_RESOURCES_IN_COLOMN - 1) {
		return false;		
	}
	if (node_row == 4 && (node_col == NUMBER_OF_RESOURCES_IN_COLOMN - 2 || node_col == NUMBER_OF_RESOURCES_IN_COLOMN - 1)) {
		return false;
	}
	return true;
}