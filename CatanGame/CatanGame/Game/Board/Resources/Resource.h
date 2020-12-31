#pragma once

#include <stdint.h>

enum class ResourceType
{
	NONE,
	WHEAT,
	CLAY,
	SHEEP,
	TREE,
	STONE,
	NONE_PORT,
	WHEAT_PORT,
	CLAY_PORT,
	SHEEP_PORT,
	TREE_PORT,
	STONE_PORT,

};

class Resource final
{
public:
	explicit Resource(const ResourceType resource_type, const int resource_number);
	ResourceType get_resource_type() const;
	uint32_t get_resource_number() const;

private:
	ResourceType m_resource_type;
	uint32_t m_resource_number;
};
