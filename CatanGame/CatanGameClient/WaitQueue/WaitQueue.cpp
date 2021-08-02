#include "WaitQueue.h"

std::string WaitQueue::front() {
	WaitForSingleObject(m_event.get_event(), INFINITE);
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_commands.front();
}

void WaitQueue::pop(){
	WaitForSingleObject(m_event.get_event(), INFINITE);
	std::lock_guard<std::mutex> lock(m_mutex);
	m_commands.pop();
	if (m_commands.empty()) {
		m_event.reset_event();
	}
}

void WaitQueue::push(std::string command){
	std::lock_guard<std::mutex> lock(m_mutex);
	m_commands.push(std::move(command));
	m_event.set_event();
}

std::string WaitQueue::pop_and_front(){
	WaitForSingleObject(m_event.get_event(), INFINITE);
	std::lock_guard<std::mutex> lock(m_mutex);
	std::string front = m_commands.front();
	m_commands.pop();
	if (m_commands.empty()) {
		m_event.reset_event();
	}
	return front;
}