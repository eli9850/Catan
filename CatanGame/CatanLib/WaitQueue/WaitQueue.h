#pragma once

#include "pch.h"

namespace QueueUtils {

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
		WinUtils::Event m_event;
	};
}