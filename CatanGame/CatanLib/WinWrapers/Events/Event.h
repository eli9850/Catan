#pragma once

#include "pch.h"

namespace WinUtils {

	class Event
	{
	public:
		Event();
		void set_event();
		void reset_event();
		const HANDLE& get_event() const;

	private:
		HANDLE m_event;
	};
}