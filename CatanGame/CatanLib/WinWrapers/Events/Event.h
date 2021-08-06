#pragma once

#include <Windows.h>
#include <string>

namespace CatanUtils
{
	namespace WinUtils
	{
		class Event final
		{
		public:
			explicit Event(std::wstring name, const bool is_manual_reset = true,
			               const bool initial_state = false,
			               const LPSECURITY_ATTRIBUTES attr = nullptr);
			explicit Event(const bool is_manual_reset = true, const bool initial_state = false,
			               const LPSECURITY_ATTRIBUTES attr = nullptr);
			void set_event();
			void reset_event();
			const HANDLE& get() const;

		private:
			HANDLE m_event;
			std::wstring m_name;
		};
	}
}
