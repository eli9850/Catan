#include "Resource.h"
#include "Exceptions/ResourceExceptions.h"
#include "Utils/Consts.h"

Resource::Resource(const ResourceType resource_type, const int resource_number):
	m_resource_type(resource_type), m_resource_number(resource_number)
{
	if (m_resource_number < MIN_RESOURCE_NUMBER || m_resource_number > MAX_RESOURCE_NUMBER)
	{
		throw WrongResourceNumber("The number is invalid");
	}
	if (m_resource_type < ResourceType::NONE || m_resource_type > ResourceType::STONE_PORT)
	{
		throw ResourceDoesNotExists("The resource is invalid");
	}
}

ResourceType Resource::get_resource_type() const
{
	return m_resource_type;
}

uint32_t Resource::get_resource_number() const
{
	return m_resource_number;
}
