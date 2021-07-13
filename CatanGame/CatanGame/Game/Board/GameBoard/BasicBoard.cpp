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
	
	for (auto i = 0; i < NUMBER_OF_RESOURCES_IN_COLUMN; i++) {
		for (auto j = 0; j < NUMBER_OF_RESOURCES_IN_ROW; j++) {
			to_string << std::to_string(static_cast<uint8_t>(m_resources[i][j]->get_resource_type()))
				<< "," << m_resources[i][j]->get_resource_number() << ";";
		}
	}
	to_string << "\n";
	for (auto i = 0; i < NUMBER_OF_NODES_IN_COLUMN; i++) {
		for (auto j = 0; j < NUMBER_OF_NODES_IN_ROW; j++) {
			if (m_nodes[i][j]->get_structure() == nullptr) {
				to_string << "0,5;";
			}
			else {
				to_string << std::to_string(static_cast<uint8_t>(m_nodes[i][j]->get_structure_type())) << "," <<
					std::to_string(static_cast<uint8_t>(m_nodes[i][j]->get_structure()->get_player()))
					<< ";";
			}

		}
	}
	to_string << "\n";
	for (auto i = 0; i < NUMBER_OF_EDGES_IN_COLUMN; i++){
		for (auto j = 0; j < NUMBER_OF_EDGES_IN_ROW; j++) {
			if (m_edges[i][j] == nullptr) {
				to_string << "5;";
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
		for (int j = 0; j < NUMBER_OF_RESOURCES_IN_COLUMN; j++)
		{
			if (is_valid_resource_index(i, j)) {
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
	for (int i = 0; i < NUMBER_OF_NODES_IN_COLUMN; i++)
	{
		for (int j = 0; j < NUMBER_OF_NODES_IN_ROW; j++)
		{
			m_nodes[i][j] = std::make_shared<Node>();
		}
	}
}

void BasicBoard::set_robber_number(const std::pair<uint32_t, uint32_t> resource_number)
{
	if (!is_valid_resource_index(resource_number.first, resource_number.second)) {
		throw InvalidNodeIndex("You are trying to put the robber in an invalid node");
	}
	m_resources[m_robber_resource_number.first][m_robber_resource_number.second].get()->set_is_robber_on(false);
	m_resources[resource_number.first][resource_number.second].get()->set_is_robber_on(true);
	m_robber_resource_number = resource_number;
}

void BasicBoard::create_edge(const uint8_t edge_row, const uint8_t edge_col, const PlayerType player_type)
{
	if (edge_row % 2 == 0 && edge_col == NUMBER_OF_EDGES_IN_COLUMN) {
		throw InvalidEdgeIndex("The index of the edge is invalid");
	}
	if (edge_row % 2 == 1 && edge_col % 2 == 1) {
		throw InvalidEdgeIndex("The index of the edge is invalid");
	}
	if (m_edges[edge_row][edge_col] != nullptr) {
		throw InvalidEdgeIndex("There is already an existing edge in that place");
	}
	m_edges[edge_row][edge_col] = std::make_shared<Edge>(player_type);
}

void BasicBoard::create_settlement(const uint8_t node_row, const uint8_t node_col, const PlayerType player_type) {
	if (!is_valid_node_index(node_row, node_col)) {
		throw InvalidNodeIndex("The index of the node is invalid");
	}
	if (m_nodes[node_row][node_col]->get_structure_type() != StructureType::NONE) {
		throw InvalidNodeIndex("There is already an existing structure in that place");
	}
	m_nodes[node_row][node_col]->set_structure(std::make_unique<Settlement>(
		player_type,
		get_node_adjacent_resources(node_row, node_col)));
	m_nodes[node_row][node_col]->set_structure_type(StructureType::SETTLEMENT);
}

void BasicBoard::upgrade_settlement_to_city(const uint8_t node_row, const uint8_t node_col) {
	auto structure = m_nodes[node_row][node_col];
	if (structure->get_structure_type() != StructureType::SETTLEMENT) {
		throw NotSettelemnt("You are trying to create city without settlement");
	}
	m_nodes[node_row][node_col]->set_structure(std::make_unique<City>(
		structure->get_structure()->get_player(),
		get_node_adjacent_resources(node_row, node_col)));
	m_nodes[node_row][node_col]->set_structure_type(StructureType::CITY);
	
}

bool BasicBoard::is_valid_resource_index(const uint8_t resource_row, const uint8_t resource_col) const {
	if (resource_row == 0 && (resource_col == 0 || resource_col == 1)) {
		return false;
	}
	if (resource_row == 1 && resource_col == 0) {
		return false;
	}
	if (resource_row == 3 && resource_col == NUMBER_OF_RESOURCES_IN_COLUMN - 1) {
		return false;		
	}
	if (resource_row == 4 && (resource_col == NUMBER_OF_RESOURCES_IN_COLUMN - 2 || resource_col == NUMBER_OF_RESOURCES_IN_COLUMN - 1)) {
		return false;
	}
	return true;
}

bool BasicBoard::is_valid_node_index(const uint8_t node_row, const uint8_t node_col) const {
	if (node_row == 0 && (node_col < 4)) {
		return false;
	}
	if (node_row == 1 && node_col < 2) {
		return false;
	}
	if (node_row == 4 && node_col > NUMBER_OF_RESOURCES_IN_COLUMN - 1 - 2) {
		return false;
	}
	if (node_row == 5 && node_col > NUMBER_OF_RESOURCES_IN_COLUMN - 1 - 4) {
		return false;
	}
	return true;
}

bool BasicBoard::is_valid_edge_index(const uint8_t edge_row, const uint8_t edge_col) const {
	if (edge_row % 2 == 1 && edge_col % 2 == 1) {
		return false;
	}
	if (edge_row % 2 == 0) {
		if (edge_col > NUMBER_OF_EDGES_IN_ROW - 1 - 1) {
			return false;
		}
		if (edge_col > std::max(static_cast<int>(NUMBER_OF_EDGES_IN_ROW - 1), 15 - edge_row) || edge_col < std::min(0, 4 - edge_row)) {
			return false;
		}
	}
	else {
		if (edge_col > NUMBER_OF_EDGES_IN_ROW - 1) {
			return false;
		}
		if (edge_col > std::max(static_cast<int>(NUMBER_OF_EDGES_IN_ROW - 1), 15 - edge_row) || edge_col < std::min(0, 5 - edge_row)) {
			return false;
		}
	}

	return true;
}

std::shared_ptr<Resource> BasicBoard::get_resource(const uint8_t row_number, const uint8_t col_number) const {
	if (!is_valid_resource_index(row_number, col_number)) {
		throw InvalidResourceIndex("The index is invalid");
	}
	if (row_number < 0 || row_number >= NUMBER_OF_EDGES_IN_ROW || col_number < 0 || col_number >= NUMBER_OF_EDGES_IN_COLUMN) {
		throw InvalidResourceIndex("The index is invlaid");
	}
	return m_resources[row_number][col_number];
}

std::shared_ptr<Edge> BasicBoard::get_edge(const uint8_t row_number, const uint8_t col_number) const {
	if (!is_valid_edge_index(row_number, col_number)) {
		throw InvalidEdgeIndex("The index is invlaid");
	}
	if (row_number < 0 || row_number >= NUMBER_OF_EDGES_IN_ROW || col_number < 0 || col_number >= NUMBER_OF_EDGES_IN_COLUMN) {
		throw InvalidEdgeIndex("The index is invlaid");
	}
	return m_edges[row_number][col_number];
}

std::shared_ptr<Node> BasicBoard::get_node(const uint8_t row_number, const uint8_t col_number) const {
	if (!is_valid_node_index(row_number, col_number)) {
		throw InvalidNodeIndex("The index is invlaid");
	}
	if (row_number < 0 || row_number >= NUMBER_OF_NODES_IN_COLUMN || col_number < 0 || col_number >= NUMBER_OF_NODES_IN_ROW) {
		throw InvalidNodeIndex("The index is invlaid");
	}
	return m_nodes[row_number][col_number];
}

std::vector<std::shared_ptr<Edge>> BasicBoard::get_edge_adjacent_edges(const uint8_t row_number, const uint8_t col_number) const {
	std::vector<std::shared_ptr<Edge>> adjacent_edges;
	if (row_number == 0) {
		try {
			if (get_edge(row_number, col_number + 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number, col_number - 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number, col_number - 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number, col_number - 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number + 1, col_number) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number + 1, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
	}
	else if (row_number == 1) {
		try {
			if (get_edge(row_number - 1, col_number - 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number - 1, col_number - 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number - 1, col_number) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number - 1, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number + 1, col_number - 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number + 1, col_number - 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number + 1, col_number) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number + 1, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
	}
	else if (row_number % 2 == 1) {
		try {
			if (get_edge(row_number - 1, col_number) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number - 1, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number - 1, col_number + 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number - 1, col_number + 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number + 1, col_number - 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number + 1, col_number - 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number + 1, col_number) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number + 1, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
	}
	else {
		try {
			if (get_edge(row_number, col_number + 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number, col_number - 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_edge(row_number, col_number - 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number, col_number - 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (col_number % 2 == 0) {
				if (get_edge(row_number - 1, col_number) != nullptr) {
					adjacent_edges.push_back(get_edge(row_number - 1, col_number));
				}
			}
			else{
				if (get_edge(row_number - 1, col_number + 1) != nullptr) {
					adjacent_edges.push_back(get_edge(row_number - 1, col_number + 1));
				}
			}
			
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (col_number % 2 == 0) {
				if (get_edge(row_number + 1, col_number) != nullptr) {
					adjacent_edges.push_back(get_edge(row_number + 1, col_number));
				}
			}
			else {
				if (get_edge(row_number + 1, col_number - 1) != nullptr) {
					adjacent_edges.push_back(get_edge(row_number + 1, col_number - 1));
				}
			}
			
		}
		catch (...) {
			// out of index, not relevant
		}
	}

	return adjacent_edges;
}

std::vector<std::shared_ptr<Node>> BasicBoard::get_edge_adjacent_nodes(const uint8_t row_number, const uint8_t col_number) const {
	std::vector<std::shared_ptr<Node>> adjacent_nodes;
	if (row_number % 2 == 0) {
		try {
			if (get_node(row_number / 2, col_number)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(row_number / 2, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_node(row_number / 2, col_number + 1)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(row_number / 2, col_number + 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
	}
	else if (row_number == 1) {
		try {
			if (get_node(0, col_number)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(0, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_node(1, col_number + 1)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(1, col_number + 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
	}
	else {
		try {
			if (get_node(row_number / 2 - 1, col_number + 1)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(row_number / 2 - 1, col_number + 1));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			if (get_node(row_number / 2 + 1, col_number)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(row_number / 2 + 1, col_number));
			}
		}
		catch (...) {
			// out of index, not relevant
		}
	}

	return adjacent_nodes;

}

std::vector<std::shared_ptr<Edge>> BasicBoard::get_node_adjacent_edges(const uint8_t row_number, const uint8_t col_number) const {
	
	std::vector<std::shared_ptr<Edge>> adjacent_edges;
	try {
		if (get_edge(row_number * 2, col_number - 1) != nullptr) {
			adjacent_edges.push_back(get_edge(row_number * 2, col_number - 1));
		}
	}
	catch (...) {
		// out of index, not relevant
	}
	try {
		if (get_edge(row_number * 2, col_number) != nullptr) {
			adjacent_edges.push_back(get_edge(row_number * 2, col_number - 1));
		}
	}
	catch (...) {
		// out of index, not relevant
	}

	try {
		if (row_number == 0) { // speicel case when row number is zero
			if (get_edge(1, col_number) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number * 2, col_number - 1));
			}
		}
		else if (col_number % 2 == 0) { // now every thing is the same, case when column is divide by 2
			if (get_edge(row_number * 2 - 1, col_number) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number * 2 - 1, col_number));
			}
		}
		else { // case when column is not divide by 2
			if (get_edge(row_number * 2 + 1, col_number - 1) != nullptr) {
				adjacent_edges.push_back(get_edge(row_number * 2 + 1, col_number - 1));
			}
		}
	}
	catch (...) {
		// out of index, not relevant
	}

	return adjacent_edges;
}

std::vector<std::shared_ptr<Node>> BasicBoard::get_node_adjacent_nodes(const uint8_t row_number, const uint8_t col_number) const {

	std::vector<std::shared_ptr<Node>> adjacent_nodes;
	try {
		if (get_node(row_number, col_number + 1)->get_structure() != nullptr) {
			adjacent_nodes.push_back(get_node(row_number, col_number + 1));
		}
	}
	catch (...) {
		// out of index, not relevant
	}
	try {
		if (get_node(row_number, col_number - 1)->get_structure() != nullptr) {
			adjacent_nodes.push_back(get_node(row_number, col_number + 1));
		}
	}
	catch (...) {
		// out of index, not relevant
	}

	try {
		if (row_number == 0) { // speicel case when row number is zero
			if (col_number % 2 == 0) {
				if (get_node(row_number + 1, col_number)->get_structure() != nullptr) {
					adjacent_nodes.push_back(get_node(row_number, col_number + 1));
				}
			}
		}
		else if (row_number == 1) { // speicel case when row number is one
			if (col_number % 2 == 0) {
				if (get_node(row_number - 1, col_number)->get_structure() != nullptr) {
					adjacent_nodes.push_back(get_node(row_number, col_number + 1));
				}
			}
			else {
				if (get_node(row_number + 1, col_number - 1)->get_structure() != nullptr) {
					adjacent_nodes.push_back(get_node(row_number, col_number + 1));
				}
			}
		}
		else if (col_number % 2 == 0) { // now every thing is the same, case when column is divide by 2
			if (get_node(row_number - 1, col_number + 1)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(row_number, col_number + 1));
			}
		}
		else { // case when column is not divide by 2
			if (get_node(row_number + 1, col_number - 1)->get_structure() != nullptr) {
				adjacent_nodes.push_back(get_node(row_number, col_number + 1));
			}
		}
	}
	catch (...) {
		// out of index, not relevant
	}
	
	return adjacent_nodes;
}

std::vector<std::shared_ptr<Resource>> BasicBoard::get_node_adjacent_resources(const uint8_t row_number, const uint8_t col_number) const {
	std::vector<std::shared_ptr<Resource>> adjacent_resources;
	if (row_number == 0) {
		try {
			adjacent_resources.push_back(get_resource(row_number, col_number / 2));
		}
		catch (...) {
			// out of index, not relevant
		}
		if (col_number % 2 == 0) {
			try {
				adjacent_resources.push_back(get_resource(row_number, col_number / 2 - 1));
			}
			catch (...) {
				// out of index, not relevant
			}
		}
	}
	else if (col_number % 2 == 0) {
		try {
			adjacent_resources.push_back(get_resource(row_number - 1, col_number / 2 - 1));
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			adjacent_resources.push_back(get_resource(row_number - 1, col_number / 2));
		}
		catch (...) {
			// out of index, not relevant
		}try {
			adjacent_resources.push_back(get_resource(row_number, col_number / 2 - 1));
		}
		catch (...) {
			// out of index, not relevant
		}
	}
	else if (col_number % 2 == 1) {
		try {
			adjacent_resources.push_back(get_resource(row_number - 1, col_number / 2 - 1));
		}
		catch (...) {
			// out of index, not relevant
		}
		try {
			adjacent_resources.push_back(get_resource(row_number, col_number / 2));
		}
		catch (...) {
			// out of index, not relevant
		}try {
			adjacent_resources.push_back(get_resource(row_number, col_number / 2 - 1));
		}
		catch (...) {
			// out of index, not relevant
		}
	}
	return adjacent_resources;
}