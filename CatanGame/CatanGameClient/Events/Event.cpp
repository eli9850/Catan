#include "Event.h"
#include "../CatanGame/Exceptions/EventsExceptions.h"

Event::Event() {
	m_event = CreateEvent(nullptr, true, false, nullptr);
	if (m_event == nullptr) {
		throw CreateEventException("Failed to create event with error: " + std::to_string(GetLastError()));
	}
}

void Event::set_event() {
	if (!SetEvent(m_event)) {
		throw SetEventException("Failed to set event with error: " + std::to_string(GetLastError()));
	}
}

void Event::reset_event() {
	if (!ResetEvent(m_event)) {
		throw SetEventException("Failed to reset event with error: " + std::to_string(GetLastError()));
	}
}

const HANDLE& Event::get_event() const{
	return m_event;
}
