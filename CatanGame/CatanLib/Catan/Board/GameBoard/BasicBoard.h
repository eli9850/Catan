#pragma once

#include <vector>
#include <array>
#include <memory>
#include <string>

#include "Catan/Board/GameBoard/IBoard.h"
#include "Catan/Board/Edges/Edge.h"
#include "Catan/Board/Nodes/Node.h"
#include "Catan/Board/Resources/Resource.h"
#include "Catan/Board/Structures/IStructure.h"
#include "Catan/CatanConsts.h"


namespace CatanUtils {

	class BasicBoard : public IBoard
	{
	public:
		void create_board() override;
		std::string to_string() const override;

		const std::shared_ptr<Resource>& get_resource(const uint32_t row_number, const uint32_t col_number) const;
		const std::shared_ptr<Edge>& get_edge(const uint32_t row_number, const uint32_t col_number) const;
		const std::shared_ptr<Node>& get_node(const uint32_t row_number, const uint32_t col_number) const;
		const std::array<std::array<std::shared_ptr<Node>, CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW>, CatanUtils::Consts::NUMBER_OF_NODES_IN_COLUMN>& get_nodes() const;
		const std::array<std::array<std::shared_ptr<Edge>, CatanUtils::Consts::NUMBER_OF_EDGES_IN_ROW>, CatanUtils::Consts::NUMBER_OF_EDGES_IN_COLUMN>& get_edges() const;
		const std::array<std::array<std::shared_ptr<Resource>, CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW>, CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_COLUMN>& get_resources() const;
		
		const std::pair<uint32_t, uint32_t>& get_robber_position() const;
		void set_robber_position(const std::pair<uint32_t, uint32_t> resource_number);
		
		void create_edge(const uint32_t row_number, const uint32_t col_number, const PlayerType player_type);
		void create_settlement(const uint32_t row_number, const uint32_t col_number, const PlayerType player_type);
		void upgrade_settlement_to_city(const uint32_t row_number, const uint32_t col_number);
		
		std::vector<std::shared_ptr<Edge>> get_edge_adjacent_edges(const uint32_t row_number, const uint32_t col_number) const;
		std::vector<std::shared_ptr<Node>> get_edge_adjacent_nodes(const uint32_t row_number, const uint32_t col_number) const;
		std::vector<std::shared_ptr<Edge>> get_node_adjacent_edges(const uint32_t row_number, const uint32_t col_number) const;
		std::vector<std::shared_ptr<Node>> get_node_adjacent_nodes(const uint32_t row_number, const uint32_t col_number) const;
		std::vector<std::shared_ptr<Resource>> get_node_adjacent_resources(const uint32_t row_number, const uint32_t col_number) const;
		std::vector<std::shared_ptr<Node>> get_resource_adjacent_nodes(const uint32_t row_number, const uint32_t col_number) const;
		

	private:
		bool is_valid_resource_index(const uint32_t resource_row, const uint32_t resource_col) const;
		bool is_valid_edge_index(const uint32_t edge_row, const uint32_t edge_col) const;
		bool is_valid_node_index(const uint32_t node_row, const uint32_t node_col) const;

		// This private section is for member of the board class
	private:
		std::pair<uint32_t, uint32_t> m_robber_position;
		std::array<std::array<std::shared_ptr<Edge>, CatanUtils::Consts::NUMBER_OF_EDGES_IN_ROW>, CatanUtils::Consts::NUMBER_OF_EDGES_IN_COLUMN> m_edges;
		std::array<std::array<std::shared_ptr<Node>, CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW>, CatanUtils::Consts::NUMBER_OF_NODES_IN_COLUMN> m_nodes;
		std::array<std::array<std::shared_ptr<Resource>, CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW>, CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_COLUMN> m_resources;
	};

}
