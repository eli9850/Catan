#pragma once

#include <queue>
#include <mutex>

#include "Events/Event.h"

class WaitQueue
{

public:
	void push(std::string command);
	void pop();
	std::string front(); 
	std::string pop_and_front();


private:
	std::mutex m_mutex;
	std::queue<std::string> m_commands;
	Event m_event;
};

