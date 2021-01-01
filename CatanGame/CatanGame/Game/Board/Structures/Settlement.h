#pragma once
#include <memory>

#include "Game/Board/Resources/Resource.h"

enum class PlayerType;

enum class SettlementCapability
{
	NONE,
	TREE_PORT,
	SHEEP_PORT,
	CLAY_PORT,
	WHEAT_PORT,
	STONE_PORT,
	ALL_PORT
};

enum class SettlementType
{
	REGULAR,
	CITY
};

class Settlement final
{
public:
	explicit Settlement(const SettlementType settlement_type, const PlayerType player_type,
	                    const SettlementCapability settlement_capability,
	                    std::shared_ptr<Resource> first_resource,
	                    std::shared_ptr<Resource> second_resource,
	                    std::shared_ptr<Resource> third_resource);
	SettlementType get_settlement_type() const;
	PlayerType get_player_type() const;
	SettlementCapability get_settlement_capability() const;
	std::shared_ptr<Resource>& get_first_resource();
	std::shared_ptr<Resource>& get_second_resource();
	std::shared_ptr<Resource>& get_third_resource();
	void upgrade_settlement();

private:
	SettlementType m_settlement_type;
	PlayerType m_player_type;
	SettlementCapability m_settlement_capability;

	std::shared_ptr<Resource> m_first_resource;
	std::shared_ptr<Resource> m_second_resource;
	std::shared_ptr<Resource> m_third_resource;
};
