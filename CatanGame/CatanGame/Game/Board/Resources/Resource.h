#pragma once

#include <stdint.h>

constexpr int MIN_RESOURCE_NUMBER = 2;
constexpr int MAX_RESOURCE_NUMBER = 12;

enum class ResourceType
{
	WHEAT,
	CLAY,
	SHEEP,
	TREE,
	STONE,
	NONE,
};

class Resource final
{
public:
	explicit Resource(const ResourceType resource_type, const uint32_t resource_number);
	ResourceType get_resource_type() const;
	uint32_t get_resource_number() const;
	bool is_robber_on() const;
	void set_is_robber_on(const bool is_robber_on);

private:
	ResourceType m_resource_type;
	uint32_t m_resource_number;
	bool m_is_robber_on;
};
