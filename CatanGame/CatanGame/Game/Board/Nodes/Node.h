#pragma once

#include <memory>

#include "Game/Board/Edges/Edge.h"
#include "Game/Board/Structures/IStructure.h"

enum class StructureType {
	NONE,
	SETTLEMENT,
	CITY
};

class Node final
{
public:
	explicit Node();
	void set_structure(std::unique_ptr<IStructure> strucure);
	std::unique_ptr<IStructure>& get_structure();
	void set_structure_type(const StructureType structure_type);
	StructureType get_structure_type() const;

private:
	std::unique_ptr<IStructure> m_structure;
	StructureType m_structure_type;
};
