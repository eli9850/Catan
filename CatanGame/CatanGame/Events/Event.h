#pragma once

#include <Windows.h>

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

