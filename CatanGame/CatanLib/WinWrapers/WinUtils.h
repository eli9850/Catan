#pragma once

#include <vector>

#include "WinWrapers/Events/Event.h"

namespace CatanUtils {

	namespace WinUtils {
		void wait_for_multiple_objects(uint32_t number_of_objects, const std::vector<Event>& events, bool is_all, uint32_t timeout);
		void wait_for_single_object(const Event& event, uint32_t timeout);
	}
}

