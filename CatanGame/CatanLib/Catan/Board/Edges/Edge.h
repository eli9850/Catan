#pragma once

#include "Catan/CatanConsts.h"

namespace CatanUtils {

	class Edge final
	{
	public:
		explicit Edge(const PlayerType player);
		PlayerType get_player() const;

	private:
		PlayerType m_player;
	};
}
