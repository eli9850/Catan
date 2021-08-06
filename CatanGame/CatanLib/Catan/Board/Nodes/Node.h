#pragma once

#include <memory>

#include "Catan/Board/Structures/IStructure.h"
#include "Catan/CatanConsts.h"

namespace CatanUtils
{
	class Node final
	{
	public:
		explicit Node();
		void set_structure(std::unique_ptr<IStructure> structure);
		const std::unique_ptr<IStructure>& get_structure() const;
		void set_structure_type(const StructureType structure_type);
		StructureType get_structure_type() const;

	private:
		std::unique_ptr<IStructure> m_structure;
		StructureType m_structure_type;
	};
}
