#include "Event.h"
#include "Exceptions/EventsExceptions.h"

namespace CatanUtils::WinUtils
{
	Event::Event(const bool is_manual_reset, const bool initial_state, const LPSECURITY_ATTRIBUTES attr)
	{
		m_event = CreateEvent(attr, is_manual_reset, initial_state, nullptr);
		if (m_event == nullptr)
		{
			throw CreateEventException(
				"Failed to create event with error: " + std::to_string(GetLastError()));
		}
	}

	Event::Event(std::wstring name, const bool is_manual_reset, const bool initial_state,
	             const LPSECURITY_ATTRIBUTES attr): m_name(std::move(name))
	{
		m_event = CreateEvent(attr, is_manual_reset, initial_state, m_name.c_str());

		if (m_event == nullptr)
		{
			throw CreateEventException(
				"Failed to create event with error: " + std::to_string(GetLastError()));
		}
	}

	void Event::set_event()
	{
		if (!SetEvent(m_event))
		{
			throw SetEventException(
				"Failed to set event with error: " + std::to_string(GetLastError()));
		}
	}

	void Event::reset_event()
	{
		if (!ResetEvent(m_event))
		{
			throw SetEventException(
				"Failed to reset event with error: " + std::to_string(GetLastError()));
		}
	}

	const HANDLE& Event::get() const
	{
		return m_event;
	}
}
