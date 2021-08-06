#pragma once

#include <string>

namespace CatanUtils
{
	class IBoard
	{
	public:
		virtual std::string to_string() const = 0;
		virtual void create_board() = 0;
	};
}
