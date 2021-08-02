#pragma once

#include <stdint.h>
#include <vector>

#include "Events/Event.h"


namespace WinUtils {
	void wait_for_multiple_objects(uint32_t number_of_objects, const std::vector<Event>& events, bool is_all, uint32_t timeout);
}
