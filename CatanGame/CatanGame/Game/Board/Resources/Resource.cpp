#include "Resource.h"
#include "Exceptions/ResourceExceptions.h"
#include "Utils/Consts.h"

Resource::Resource(const ResourceType resource_type, const uint32_t resource_number):
	m_resource_type(resource_type), m_resource_number(resource_number), m_is_robber_on(false)
{
	if ((m_resource_number < MIN_RESOURCE_NUMBER || m_resource_number > MAX_RESOURCE_NUMBER) && m_resource_number != 0)
	{
		throw WrongResourceNumber("The number is invalid");
	}
}

ResourceType Resource::get_resource_type() const
{
	return m_resource_type;
}

void Resource::set_is_robber_on(const bool is_robber_on) {
	m_is_robber_on = is_robber_on;
}

uint32_t Resource::get_resource_number() const
{
	return m_resource_number;
}

bool Resource::is_robber_on() const {
	return m_is_robber_on;
}
