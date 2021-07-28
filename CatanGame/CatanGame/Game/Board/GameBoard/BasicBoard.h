#pragma once

#include <vector>
#include <array>
#include <memory>
#include <string>

#include "Utils/Consts.h"
#include "IBoard.h"

#include "Game/Board/Nodes/Node.h"
#include "Game/Board/Edges/Edge.h"
#include "Game/Board/Resources/Resource.h"

constexpr uint32_t NUMBER_OF_RESOURCES_IN_ROW = 5;
constexpr uint32_t NUMBER_OF_RESOURCES_IN_COLUMN = 5;
constexpr uint32_t NUMBER_OF_NODES_IN_ROW = 12;
constexpr uint32_t NUMBER_OF_NODES_IN_COLUMN = 6;
constexpr uint32_t NUMBER_OF_EDGES_IN_ROW = 11;
constexpr uint32_t NUMBER_OF_EDGES_IN_COLUMN = 11;
constexpr uint32_t NUMBER_OF_VALID_NODE = 19;


class BasicBoard : public IBoard
{
public:
	std::string to_string() const;
	void create_board() override;

	void create_settlement(const uint8_t node_row, const uint8_t node_col, const PlayerType player_type);
	void upgrade_settlement_to_city(const uint8_t node_row, const uint8_t node_col);
	void create_edge(const uint8_t edge_row, const uint8_t edge_col, const PlayerType player_type);
	void set_robber_number(const std::pair<uint32_t, uint32_t> resource_number);
	std::shared_ptr<Resource> get_resource(const uint8_t row_number, const uint8_t col_number) const;
	std::shared_ptr<Edge> get_edge(const uint8_t row_number, const uint8_t col_number) const;
	std::shared_ptr<Node> get_node(const uint8_t row_number, const uint8_t col_number) const;
	std::vector<std::shared_ptr<Edge>> get_edge_adjacent_edges(const uint8_t row_number, const uint8_t col_number) const;
	std::vector<std::shared_ptr<Node>> get_edge_adjacent_nodes(const uint8_t row_number, const uint8_t col_number) const;
	std::vector<std::shared_ptr<Edge>> get_node_adjacent_edges(const uint8_t row_number, const uint8_t col_number) const;
	std::vector<std::shared_ptr<Node>> get_node_adjacent_nodes(const uint8_t row_number, const uint8_t col_number) const;
	std::vector<std::shared_ptr<Resource>> get_node_adjacent_resources(const uint8_t row_number, const uint8_t col_number) const;
	const std::array<std::array<std::shared_ptr<Node>, NUMBER_OF_NODES_IN_ROW>, NUMBER_OF_NODES_IN_COLUMN>& get_nodes() const {
		return m_nodes;
	}

private:
	bool is_valid_resource_index(const uint8_t resource_row, const uint8_t resource_col) const;
	bool is_valid_edge_index(const uint8_t edge_row, const uint8_t edge_col) const;
	bool is_valid_node_index(const uint8_t node_row, const uint8_t node_col) const;

	// This private section is for member of the board class
private:
	std::pair<uint32_t, uint32_t> m_robber_resource_number;
	std::array<std::array<std::shared_ptr<Edge>, NUMBER_OF_EDGES_IN_ROW>, NUMBER_OF_EDGES_IN_COLUMN> m_edges;
	std::array<std::array<std::shared_ptr<Node>, NUMBER_OF_NODES_IN_ROW>, NUMBER_OF_NODES_IN_COLUMN> m_nodes;
	std::array<std::array<std::shared_ptr<Resource>, NUMBER_OF_RESOURCES_IN_ROW>, NUMBER_OF_RESOURCES_IN_COLUMN> m_resources;
};
