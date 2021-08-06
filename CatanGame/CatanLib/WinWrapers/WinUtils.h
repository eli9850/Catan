#pragma once

#include <vector>

#include "WinWrapers/Events/Event.h"

namespace CatanUtils
{
	namespace WinUtils
	{
		void wait_for_multiple_objects(const std::vector<Event>& events, const bool is_all,
		                               const uint32_t timeout);
		void wait_for_single_object(const Event& event, const uint32_t timeout);
	}
}
