#include "BasicBoard.h"

#include <sstream>

#include "Utils/RandomUtils.h"
#include "Exceptions/BoardExceptions.h"
#include "Catan/Board/Structures/Settlement.h"
#include "Catan/Board/Structures/City.h"

namespace CatanUtils
{
	void BasicBoard::create_board()
	{
		std::array<uint32_t, CatanUtils::Consts::NUMBER_OF_VALID_NODES -
		           CatanUtils::Consts::NUMBER_OF_DESERT_RESOURCES> resource_numbers = {
			2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12
		};
		std::array<ResourceType, CatanUtils::Consts::NUMBER_OF_VALID_NODES> resource_types = {
			ResourceType::NONE,
			ResourceType::SHEEP, ResourceType::SHEEP, ResourceType::SHEEP, ResourceType::SHEEP,
			ResourceType::WHEAT, ResourceType::WHEAT, ResourceType::WHEAT, ResourceType::WHEAT,
			ResourceType::TREE, ResourceType::TREE, ResourceType::TREE, ResourceType::TREE,
			ResourceType::STONE, ResourceType::STONE, ResourceType::STONE,
			ResourceType::CLAY, ResourceType::CLAY, ResourceType::CLAY,
		};

		CatanUtils::RandomUtils::shuffle_array(resource_numbers);
		CatanUtils::RandomUtils::shuffle_array(resource_types);

		uint32_t resource_type_index = 0;
		uint32_t resource_number_index = 0;
		for (uint32_t i = 0; i < CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW; i++)
		{
			for (uint32_t j = 0; j < CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_COLUMN; j++)
			{
				if (is_valid_resource_index(i, j))
				{
					if (resource_types.at(resource_type_index) == ResourceType::NONE)
					{
						m_resources.at(i).at(j) = std::make_shared<Resource>(
							ResourceType::NONE, CatanUtils::Consts::NUMBER_OF_DESERT_RESOURCE);
						m_resources.at(i).at(j)->set_is_robber_on(true);
						m_robber_position.first = i;
						m_robber_position.second = j;
						resource_type_index++;
						continue;
					}
					m_resources.at(i).at(j) = std::make_shared<Resource>(
						resource_types.at(resource_type_index),
						resource_numbers.at(resource_number_index));
					resource_type_index++;
					resource_number_index++;
				}
				else
				{
					m_resources.at(i).at(j) = std::make_shared<Resource>(
						ResourceType::NONE, CatanUtils::Consts::NUMBER_OF_DESERT_RESOURCE);
				}
			}
		}
		for (uint32_t i = 0; i < CatanUtils::Consts::NUMBER_OF_NODES_IN_COLUMN; i++)
		{
			for (uint32_t j = 0; j < CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW; j++)
			{
				m_nodes.at(i).at(j) = std::make_shared<Node>();
			}
		}
	}

	std::string BasicBoard::to_string() const
	{
		std::stringstream to_string;

		for (uint32_t i = 0; i < CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_COLUMN; i++)
		{
			for (uint32_t j = 0; j < CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW; j++)
			{
				to_string << std::to_string(
						static_cast<uint32_t>(m_resources.at(i).at(j)->get_resource_type()))
					<< "," << m_resources.at(i).at(j)->get_resource_number() << ";";
			}
		}
		to_string << "\n";
		for (uint32_t i = 0; i < CatanUtils::Consts::NUMBER_OF_NODES_IN_COLUMN; i++)
		{
			for (uint32_t j = 0; j < CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW; j++)
			{
				if (m_nodes.at(i).at(j)->get_structure() == nullptr)
				{
					to_string << std::to_string(CatanUtils::Consts::EMPTY_NODE) << ",";
					to_string << std::to_string(CatanUtils::Consts::NON_EXISTS_PLAYER_NUMBER) << ";";
				}
				else
				{
					to_string << std::to_string(
							static_cast<uint32_t>(m_nodes.at(i).at(j)->get_structure_type())) << "," <<
						std::to_string(
							static_cast<uint32_t>(m_nodes.at(i).at(j)->get_structure()->get_player())) <<
						";";
				}
			}
		}
		to_string << "\n";
		for (uint32_t i = 0; i < CatanUtils::Consts::NUMBER_OF_EDGES_IN_COLUMN; i++)
		{
			for (uint32_t j = 0; j < CatanUtils::Consts::NUMBER_OF_EDGES_IN_ROW; j++)
			{
				if (m_edges.at(i).at(j) == nullptr)
				{
					to_string << std::to_string(CatanUtils::Consts::NON_EXISTS_PLAYER_NUMBER) << ";";
				}
				else
				{
					to_string << std::to_string(static_cast<uint32_t>(m_edges.at(i).at(j)->get_player()))
						<< ";";
				}
			}
		}
		to_string << "\n" << std::to_string(m_robber_position.first) << "," << std::to_string(
			m_robber_position.second);

		return to_string.str();
	}

	const std::shared_ptr<Resource>& BasicBoard::get_resource(const uint32_t row_number,
	                                                          const uint32_t col_number) const
	{
		if (!is_valid_resource_index(row_number, col_number))
		{
			throw InvalidResourceIndex("The index is invalid");
		}
		return m_resources.at(row_number).at(col_number);
	}

	const std::shared_ptr<Edge>& BasicBoard::get_edge(const uint32_t row_number,
	                                                  const uint32_t col_number) const
	{
		if (!is_valid_edge_index(row_number, col_number))
		{
			throw InvalidEdgeIndex("The index is invalid");
		}
		return m_edges.at(row_number).at(col_number);
	}

	const std::shared_ptr<Node>& BasicBoard::get_node(const uint32_t row_number,
	                                                  const uint32_t col_number) const
	{
		if (!is_valid_node_index(row_number, col_number))
		{
			throw InvalidNodeIndex("The index is invalid");
		}
		return m_nodes.at(row_number).at(col_number);
	}

	const std::array<std::array<std::shared_ptr<Node>, CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW>,
	                 CatanUtils::Consts::NUMBER_OF_NODES_IN_COLUMN>& BasicBoard::get_nodes() const
	{
		return m_nodes;
	}

	const std::array<std::array<std::shared_ptr<Edge>, CatanUtils::Consts::NUMBER_OF_EDGES_IN_ROW>,
	                 CatanUtils::Consts::NUMBER_OF_EDGES_IN_COLUMN>& BasicBoard::get_edges() const
	{
		return m_edges;
	}

	const std::array<std::array<std::shared_ptr<Resource>, CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW
	                 >, CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_COLUMN>&
	BasicBoard::get_resources() const
	{
		return m_resources;
	}

	const std::pair<uint32_t, uint32_t>& BasicBoard::get_robber_position() const
	{
		return m_robber_position;
	}

	void BasicBoard::set_robber_position(const std::pair<uint32_t, uint32_t> resource_number)
	{
		if (!is_valid_resource_index(resource_number.first, resource_number.second))
		{
			throw InvalidResourceIndex("You are trying to put the robber in an invalid node");
		}
		if (resource_number.first == m_robber_position.first &&
			resource_number.second == m_robber_position.second)
		{
			throw InvalidResourceIndex("You are trying to put the robber in the same position");
		}
		m_resources.at(m_robber_position.first).at(m_robber_position.second)->set_is_robber_on(false);
		m_resources.at(resource_number.first).at(resource_number.second)->set_is_robber_on(true);
		m_robber_position = resource_number;
	}

	void BasicBoard::create_edge(const uint32_t row_number, const uint32_t col_number,
	                             const PlayerType player_type)
	{
		if (!is_valid_edge_index(row_number, col_number))
		{
			throw InvalidEdgeIndex("The index of the edge is invalid");
		}

		if (m_edges.at(row_number).at(col_number) != nullptr)
		{
			throw InvalidEdgeIndex("There is already an existing edge in that place");
		}

		m_edges.at(row_number).at(col_number) = std::make_shared<Edge>(player_type);
	}

	void BasicBoard::create_settlement(const uint32_t row_number, const uint32_t col_number,
	                                   const PlayerType player_type)
	{
		if (!is_valid_node_index(row_number, col_number))
		{
			throw InvalidNodeIndex("The index of the node is invalid");
		}
		if (m_nodes.at(row_number).at(col_number)->get_structure_type() != StructureType::NONE)
		{
			throw InvalidNodeIndex("There is already an existing structure in that place");
		}
		m_nodes.at(row_number).at(col_number)->set_structure(std::make_unique<Settlement>(
			player_type,
			get_node_adjacent_resources(row_number, col_number)));
		m_nodes.at(row_number).at(col_number)->set_structure_type(StructureType::SETTLEMENT);
	}

	void BasicBoard::upgrade_settlement_to_city(const uint32_t row_number, const uint32_t col_number)
	{
		auto& structure = m_nodes.at(row_number).at(col_number);
		if (structure->get_structure_type() != StructureType::SETTLEMENT)
		{
			throw NotSettelemnt("You are trying to create city without settlement");
		}
		m_nodes.at(row_number).at(col_number)->set_structure(std::make_unique<City>(
			structure->get_structure()->get_player(),
			get_node_adjacent_resources(row_number, col_number)));
		m_nodes.at(row_number).at(col_number)->set_structure_type(StructureType::CITY);
	}

	std::vector<std::shared_ptr<Edge>> BasicBoard::get_edge_adjacent_edges(
		const uint32_t row_number, const uint32_t col_number) const
	{
		if (!is_valid_edge_index(row_number, col_number))
		{
			throw InvalidEdgeIndex("The index is invalid");
		}

		std::vector<std::shared_ptr<Edge>> adjacent_edges;
		if (row_number == 0)
		{
			try
			{
				if (get_edge(row_number, col_number + 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number, col_number - 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number, col_number - 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number, col_number - 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number + 1, col_number) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number + 1, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}
		else if (row_number == 1)
		{
			try
			{
				if (get_edge(row_number - 1, col_number - 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number - 1, col_number - 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number - 1, col_number) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number - 1, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number + 1, col_number - 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number + 1, col_number - 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number + 1, col_number) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number + 1, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}
		else if (row_number % 2 == 1)
		{
			try
			{
				if (get_edge(row_number - 1, col_number) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number - 1, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number - 1, col_number + 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number - 1, col_number + 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number + 1, col_number - 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number + 1, col_number - 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number + 1, col_number) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number + 1, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}
		else
		{
			try
			{
				if (get_edge(row_number, col_number + 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number, col_number - 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_edge(row_number, col_number - 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number, col_number - 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (col_number % 2 == 0)
				{
					if (get_edge(row_number - 1, col_number) != nullptr)
					{
						adjacent_edges.push_back(get_edge(row_number - 1, col_number));
					}
				}
				else
				{
					if (get_edge(row_number - 1, col_number + 1) != nullptr)
					{
						adjacent_edges.push_back(get_edge(row_number - 1, col_number + 1));
					}
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (col_number % 2 == 0)
				{
					if (get_edge(row_number + 1, col_number) != nullptr)
					{
						adjacent_edges.push_back(get_edge(row_number + 1, col_number));
					}
				}
				else
				{
					if (get_edge(row_number + 1, col_number - 1) != nullptr)
					{
						adjacent_edges.push_back(get_edge(row_number + 1, col_number - 1));
					}
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}

		return adjacent_edges;
	}

	std::vector<std::shared_ptr<Node>> BasicBoard::get_edge_adjacent_nodes(
		const uint32_t row_number, const uint32_t col_number) const
	{
		if (!is_valid_edge_index(row_number, col_number))
		{
			throw InvalidEdgeIndex("The index is invalid");
		}

		std::vector<std::shared_ptr<Node>> adjacent_nodes;
		if (row_number % 2 == 0)
		{
			try
			{
				if (get_node(row_number / 2, col_number)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(row_number / 2, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_node(row_number / 2, col_number + 1)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(row_number / 2, col_number + 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}
		else if (row_number == 1)
		{
			try
			{
				if (get_node(0, col_number)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(0, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_node(1, col_number)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(1, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}
		else
		{
			try
			{
				if (get_node(row_number / 2, col_number + 1)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(row_number / 2, col_number + 1));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				if (get_node(row_number / 2 + 1, col_number)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(row_number / 2 + 1, col_number));
				}
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}

		return adjacent_nodes;
	}

	std::vector<std::shared_ptr<Edge>> BasicBoard::get_node_adjacent_edges(
		const uint32_t row_number, const uint32_t col_number) const
	{
		if (!is_valid_node_index(row_number, col_number))
		{
			throw InvalidNodeIndex("The index is invalid");
		}

		std::vector<std::shared_ptr<Edge>> adjacent_edges;
		try
		{
			if (get_edge(row_number * 2, col_number - 1) != nullptr)
			{
				adjacent_edges.push_back(get_edge(row_number * 2, col_number - 1));
			}
		}
		catch (...)
		{
			// out of index, not relevant
		}
		try
		{
			if (get_edge(row_number * 2, col_number) != nullptr)
			{
				adjacent_edges.push_back(get_edge(row_number * 2, col_number - 1));
			}
		}
		catch (...)
		{
			// out of index, not relevant
		}

		try
		{
			if (row_number == 0)
			{
				// special case when row number is zero
				if (get_edge(1, col_number) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number * 2, col_number - 1));
				}
			}
			else if (col_number % 2 == 0)
			{
				// now every thing is the same, case when column is divide by 2
				if (get_edge(row_number * 2 - 1, col_number) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number * 2 - 1, col_number));
				}
			}
			else
			{
				// case when column is not divide by 2
				if (get_edge(row_number * 2 + 1, col_number - 1) != nullptr)
				{
					adjacent_edges.push_back(get_edge(row_number * 2 + 1, col_number - 1));
				}
			}
		}
		catch (...)
		{
			// out of index, not relevant
		}

		return adjacent_edges;
	}

	std::vector<std::shared_ptr<Node>> BasicBoard::get_node_adjacent_nodes(
		const uint32_t row_number, const uint32_t col_number) const
	{
		if (!is_valid_node_index(row_number, col_number))
		{
			throw InvalidNodeIndex("The index is invalid");
		}

		std::vector<std::shared_ptr<Node>> adjacent_nodes;
		try
		{
			if (get_node(row_number, col_number + 1)->get_structure() != nullptr)
			{
				adjacent_nodes.push_back(get_node(row_number, col_number + 1));
			}
		}
		catch (...)
		{
			// out of index, not relevant
		}
		try
		{
			if (get_node(row_number, col_number - 1)->get_structure() != nullptr)
			{
				adjacent_nodes.push_back(get_node(row_number, col_number + 1));
			}
		}
		catch (...)
		{
			// out of index, not relevant
		}

		try
		{
			if (row_number == 0)
			{
				// special case when row number is zero
				if (col_number % 2 == 0)
				{
					if (get_node(row_number + 1, col_number)->get_structure() != nullptr)
					{
						adjacent_nodes.push_back(get_node(row_number, col_number + 1));
					}
				}
			}
			else if (row_number == 1)
			{
				// special case when row number is one
				if (col_number % 2 == 0)
				{
					if (get_node(row_number - 1, col_number)->get_structure() != nullptr)
					{
						adjacent_nodes.push_back(get_node(row_number, col_number + 1));
					}
				}
				else
				{
					if (get_node(row_number + 1, col_number - 1)->get_structure() != nullptr)
					{
						adjacent_nodes.push_back(get_node(row_number, col_number + 1));
					}
				}
			}
			else if (col_number % 2 == 0)
			{
				// now every thing is the same, case when column is divide by 2
				if (get_node(row_number - 1, col_number + 1)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(row_number, col_number + 1));
				}
			}
			else
			{
				// case when column is not divide by 2
				if (get_node(row_number + 1, col_number - 1)->get_structure() != nullptr)
				{
					adjacent_nodes.push_back(get_node(row_number, col_number + 1));
				}
			}
		}
		catch (...)
		{
			// out of index, not relevant
		}

		return adjacent_nodes;
	}

	std::vector<std::shared_ptr<Resource>> BasicBoard::get_node_adjacent_resources(
		const uint32_t row_number, const uint32_t col_number) const
	{
		if (!is_valid_node_index(row_number, col_number))
		{
			throw InvalidNodeIndex("The index is invalid");
		}

		std::vector<std::shared_ptr<Resource>> adjacent_resources;
		if (row_number == 0)
		{
			try
			{
				adjacent_resources.push_back(get_resource(row_number, col_number / 2));
			}
			catch (...)
			{
				// out of index, not relevant
			}
			if (col_number % 2 == 0)
			{
				try
				{
					adjacent_resources.push_back(get_resource(row_number, col_number / 2 - 1));
				}
				catch (...)
				{
					// out of index, not relevant
				}
			}
		}
		else if (col_number % 2 == 0)
		{
			try
			{
				adjacent_resources.push_back(get_resource(row_number - 1, col_number / 2 - 1));
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				adjacent_resources.push_back(get_resource(row_number - 1, col_number / 2));
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				adjacent_resources.push_back(get_resource(row_number, col_number / 2 - 1));
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}
		else if (col_number % 2 == 1)
		{
			try
			{
				adjacent_resources.push_back(get_resource(row_number - 1, col_number / 2));
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				adjacent_resources.push_back(get_resource(row_number, col_number / 2));
			}
			catch (...)
			{
				// out of index, not relevant
			}
			try
			{
				adjacent_resources.push_back(get_resource(row_number, col_number / 2 - 1));
			}
			catch (...)
			{
				// out of index, not relevant
			}
		}
		return adjacent_resources;
	}

	std::vector<std::shared_ptr<Node>> BasicBoard::get_resource_adjacent_nodes(
		const uint32_t row_number, const uint32_t col_number) const
	{
		if (!is_valid_resource_index(row_number, col_number))
		{
			throw InvalidResourceIndex("The index is invalid");
		}

		std::vector<std::shared_ptr<Node>> adjacent_nodes;
		if (row_number == 0)
		{
			adjacent_nodes.push_back(get_node(row_number, col_number + 2 + 0));
			adjacent_nodes.push_back(get_node(row_number, col_number + 2 + 1));
			adjacent_nodes.push_back(get_node(row_number, col_number + 2 + 2));
			adjacent_nodes.push_back(get_node(row_number + 1, col_number + 2 + 0));
			adjacent_nodes.push_back(get_node(row_number + 1, col_number + 2 + 1));
			adjacent_nodes.push_back(get_node(row_number + 1, col_number + 2 + 2));
		}
		else
		{
			adjacent_nodes.push_back(get_node(row_number, col_number + 2 + 1));
			adjacent_nodes.push_back(get_node(row_number, col_number + 2 + 2));
			adjacent_nodes.push_back(get_node(row_number, col_number + 2 + 3));
			adjacent_nodes.push_back(get_node(row_number + 1, col_number + 2 + 0));
			adjacent_nodes.push_back(get_node(row_number + 1, col_number + 2 + 1));
			adjacent_nodes.push_back(get_node(row_number + 1, col_number + 2 + 2));
		}
		return adjacent_nodes;
	}

	bool BasicBoard::is_valid_resource_index(const uint32_t row_number, const uint32_t col_number) const
	{
		if (row_number < 0 || row_number > CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_COLUMN - 1)
		{
			return false;
		}
		if (col_number < 0 || col_number > CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW - 1)
		{
			return false;
		}

		if (row_number == 0 && col_number < 2)
		{
			return false;
		}
		if (row_number == 1 && col_number < 1)
		{
			return false;
		}
		if (row_number == 3 && col_number > CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW - 1 - 1)
		{
			return false;
		}
		if (row_number == 4 && col_number > CatanUtils::Consts::NUMBER_OF_RESOURCES_IN_ROW - 1 - 2)
		{
			return false;
		}
		return true;
	}

	bool BasicBoard::is_valid_node_index(const uint32_t row_number, const uint32_t col_number) const
	{
		if (row_number < 0 || row_number > CatanUtils::Consts::NUMBER_OF_NODES_IN_COLUMN - 1)
		{
			return false;
		}
		if (col_number < 0 || col_number > CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW - 1)
		{
			return false;
		}

		if (row_number != 1 && row_number != 2 && col_number ==
			CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW - 1)
		{
			return false;
		}
		if (row_number == 0 && (col_number < 4))
		{
			return false;
		}
		if (row_number == 1 && col_number < 2)
		{
			return false;
		}
		if (row_number == 4 && col_number > CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW - 1 - 3)
		{
			return false;
		}
		if (row_number == 5 && col_number > CatanUtils::Consts::NUMBER_OF_NODES_IN_ROW - 1 - 5)
		{
			return false;
		}
		return true;
	}

	bool BasicBoard::is_valid_edge_index(const uint32_t row_number, const uint32_t col_number) const
	{
		if (row_number < 0 || row_number > CatanUtils::Consts::NUMBER_OF_EDGES_IN_COLUMN - 1)
		{
			return false;
		}
		if (col_number < 0 || col_number > CatanUtils::Consts::NUMBER_OF_EDGES_IN_ROW - 1)
		{
			return false;
		}

		if (row_number % 2 == 1 && col_number % 2 == 1)
		{
			return false;
		}

		if (row_number == 0 && col_number > CatanUtils::Consts::NUMBER_OF_EDGES_IN_ROW - 1 - 1)
		{
			return false;
		}

		if (row_number < 5 && col_number > CatanUtils::Consts::NUMBER_OF_EDGES_IN_ROW - 1)
		{
			return false;
		}

		if (row_number >= 5 && col_number > 15 - row_number)
		{
			return false;
		}

		return true;
	}
}
