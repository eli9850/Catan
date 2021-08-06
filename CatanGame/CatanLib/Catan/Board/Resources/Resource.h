#pragma once

#include <cstdint>

#include "Catan/CatanConsts.h"

namespace CatanUtils
{
	class Resource final
	{
	public:
		explicit Resource(const ResourceType resource_type, const uint32_t resource_number,
		                  const bool is_robber_on = false);
		ResourceType get_resource_type() const;
		uint32_t get_resource_number() const;
		bool is_robber_on() const;
		void set_is_robber_on(const bool is_robber_on);

	private:
		ResourceType m_resource_type;
		uint32_t m_resource_number;
		bool m_is_robber_on;
	};
}
