#include "WinUtils.h"

namespace WinUtils {

	void wait_for_multiple_objects(uint32_t number_of_objects, const std::vector<Event>& events, bool is_all, uint32_t timeout) {
		std::unique_ptr<HANDLE[]> events_handles = std::make_unique<HANDLE[]>(events.size());
		for (size_t i = 0; i < events.size(); i++) {
			events_handles[i] = events.at(i).get_event();
		}
		WaitForMultipleObjects(events.size(), events_handles.get(), is_all, timeout);
	}
}
