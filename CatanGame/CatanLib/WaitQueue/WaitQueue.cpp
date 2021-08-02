#include "WaitQueue.h"

#include "Exceptions/WinExceptions.h"
#include "Exceptions/QueueExceptions.h"

namespace QueueUtils {
	
	std::string WaitQueue::front() {
		
		// wait for event in order to not lock the mutex
		try {
			WinUtils::wait_for_single_object(m_event.get_event(), INFINITE);
		}
		catch (const WinException& e){
			throw FrontQueueException("Could not get the frot element");
		}

		std::lock_guard<std::mutex> lock(m_mutex);

		// wait for event again in order to confirm that we still have an object
		try {
			WinUtils::wait_for_single_object(m_event.get_event(), INFINITE);
		}
		catch (const WinException& e) {
			throw FrontQueueException("Could not get the frot element");
		}
		return m_commands.front();
		
	}

	void WaitQueue::pop() {

		// wait for event in order to not lock the mutex
		try {
			WinUtils::wait_for_single_object(m_event.get_event(), INFINITE);
		}
		catch (const WinException& e) {
			throw PopQueueException("Could not pop queue");
		}

		std::lock_guard<std::mutex> lock(m_mutex);

		// wait for event again in order to confirm that we still have an object
		try {
			WinUtils::wait_for_single_object(m_event.get_event(), INFINITE);
		}
		catch (const WinException& e) {
			throw PopQueueException("Could not pop queue");
		}
		m_commands.pop();
		if (m_commands.empty()) {
			m_event.reset_event();
		}

	}

	void WaitQueue::push(std::string command) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_commands.push(std::move(command));
		m_event.set_event();
	}

	std::string WaitQueue::pop_and_front() {
		auto front_command = front();
		pop();
		return front_command;
	}
}