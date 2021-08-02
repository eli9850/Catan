#include "WinUtils.h"

#include <memory>

#include "Exceptions/WinExceptions.h"

namespace WinUtils {

	void wait_for_multiple_objects(uint32_t number_of_objects, const std::vector<Event>& events, bool is_all, uint32_t timeout) {
		std::unique_ptr<HANDLE[]> events_handles = std::make_unique<HANDLE[]>(events.size());
		for (size_t i = 0; i < events.size(); i++) {
			events_handles[i] = events.at(i).get_event();
		}
		auto result = WaitForMultipleObjects(events.size(), events_handles.get(), is_all, timeout);
		if (result == WAIT_TIMEOUT) {
			throw TimeoutException("Wait for multiple objects - Timeout error");
		}
		if (result == WAIT_FAILED) {
			throw FailedException("Wait for multiple objects failed with error: " + std::to_string(GetLastError()));
		}
	}

	void wait_for_single_object(const Event& event, uint32_t timeout) {
		
		auto result = WaitForSingleObject(event.get_event(), timeout);
		if (result == WAIT_TIMEOUT) {
			throw TimeoutException("Wait for single objects - Timeout error");
		}
		if (result == WAIT_FAILED) {
			throw FailedException("Wait for single objects failed with error: " + std::to_string(GetLastError()));
		}
	}
}
