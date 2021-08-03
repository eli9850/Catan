#include "Event.h"
#include "Exceptions/EventsExceptions.h"

namespace CatanUtils {

	namespace WinUtils {

		Event::Event(bool is_maual_reset, bool initial_state, LPSECURITY_ATTRIBUTES attr) {

			m_event = CreateEvent(attr, is_maual_reset, initial_state, nullptr);
			if (m_event == nullptr) {
				throw CreateEventException("Failed to create event with error: " + std::to_string(GetLastError()));
			}
		}

		Event::Event(std::wstring name, bool is_maual_reset, bool initial_state, LPSECURITY_ATTRIBUTES attr) {

			m_event = CreateEvent(attr, is_maual_reset, initial_state, name.c_str());

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

		const HANDLE& Event::get_event() const {
			return m_event;
		}
	}
}
