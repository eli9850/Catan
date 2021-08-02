#include "Node.h"

namespace CatanUtils {

	Node::Node() : m_structure(nullptr), m_structure_type(StructureType::NONE)
	{
	}

	void Node::set_structure(std::unique_ptr<IStructure> structure)
	{
		m_structure = std::move(structure);
	}

	const std::unique_ptr<IStructure>& Node::get_structure() const 
	{
		return m_structure;
	}

	StructureType Node::get_structure_type() const {
		return m_structure_type;
	}

	void Node::set_structure_type(const StructureType structure_type) {
		m_structure_type = structure_type;
	}

}
