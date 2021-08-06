#include "WinUtils.h"

#include <memory>

#include "Exceptions/WinExceptions.h"

namespace CatanUtils::WinUtils
{
	void wait_for_multiple_objects(const std::vector<Event>& events, const bool is_all,
	                               const uint32_t timeout)
	{
		const auto events_handles = std::make_unique<HANDLE[]>(events.size());
		for (size_t i = 0; i < events.size(); i++)
		{
			events_handles[i] = events.at(i).get();
		}
		const auto result = WaitForMultipleObjects(events.size(), events_handles.get(), is_all, timeout);
		if (result == WAIT_TIMEOUT)
		{
			throw TimeoutException("Wait for multiple objects - Timeout error");
		}
		if (result == WAIT_FAILED)
		{
			throw FailedException(
				"Wait for multiple objects failed with error: " + std::to_string(GetLastError()));
		}
	}

	void wait_for_single_object(const Event& event, const uint32_t timeout)
	{
		const auto result = WaitForSingleObject(event.get(), timeout);
		if (result == WAIT_TIMEOUT)
		{
			throw TimeoutException("Wait for single objects - Timeout error");
		}
		if (result == WAIT_FAILED)
		{
			throw FailedException(
				"Wait for single objects failed with error: " + std::to_string(GetLastError()));
		}
	}
}
