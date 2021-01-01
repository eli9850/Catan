#pragma once

#include <string>
#include <exception>

#define INHERIT_EXCEPTION(exception_name, parent_exception)                           \
class exception_name : public parent_exception {                                      \
public:                                                                               \
    exception_name(std::string message): parent_exception(std::move(message)) {}      \
	std::string get_message() const override {                                        \
		return (std::string(#exception_name": ") + m_message).c_str();                \
	}                                                                                 \
};

class CatanException : public std::exception
{
public:
	explicit CatanException(std::string message) : m_message(std::move(message))
	{
	}

	virtual std::string get_message() const { return m_message; }
protected:
	std::string m_message;
};
