#pragma once

#include <Windows.h>
#include <string>

namespace WinUtils {

	class Event
	{
	public:
		Event(std::wstring name, bool is_maual_reset = true, bool initial_state = false, LPSECURITY_ATTRIBUTES attr = nullptr);
		Event(bool is_maual_reset = true, bool initial_state = false, LPSECURITY_ATTRIBUTES attr = nullptr);
		void set_event();
		void reset_event();
		const HANDLE& get_event() const;

	private:
		HANDLE m_event;
		std::string m_name;
	};
}