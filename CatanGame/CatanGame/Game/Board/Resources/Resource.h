#pragma once

#include <cstdint>

enum class ResourceType
{
	NONE,
	WHEAT,
	CLAY,
	SHEEP,
	TREE,
	STONE,
};

class Resource final
{
public:
	explicit Resource(const ResourceType resource_type, const uint32_t resource_number);
	ResourceType get_resource_type() const;
	uint32_t get_resource_number() const;

private:
	ResourceType m_resource_type;
	uint32_t m_resource_number;
};
