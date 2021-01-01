#include "Settlement.h"
#include "Exceptions/SettlementExceptions.h"

Settlement::Settlement(const SettlementType settlement_type, const PlayerType player_type,
                       const SettlementCapability settlement_capability,
                       std::shared_ptr<Resource> first_resource,
                       std::shared_ptr<Resource> second_resource,
                       std::shared_ptr<Resource> third_resource):
	m_settlement_type(settlement_type),
	m_player_type(player_type),
	m_settlement_capability(settlement_capability),
	m_first_resource(std::move(first_resource)),
	m_second_resource(std::move(second_resource)),
	m_third_resource(std::move(third_resource))

{
}

SettlementCapability Settlement::get_settlement_capability() const
{
	return m_settlement_capability;
}

PlayerType Settlement::get_player_type() const
{
	return m_player_type;
}

SettlementType Settlement::get_settlement_type() const
{
	return m_settlement_type;
}

std::shared_ptr<Resource>& Settlement::get_first_resource()
{
	return m_first_resource;
}

std::shared_ptr<Resource>& Settlement::get_second_resource()
{
	return m_second_resource;
}

std::shared_ptr<Resource>& Settlement::get_third_resource()
{
	return m_third_resource;
}

void Settlement::upgrade_settlement()
{
	if (m_settlement_type == SettlementType::CITY)
	{
		throw UpgradeSettlementError("The settlement is already a city");
	}
	m_settlement_type = SettlementType::CITY;
}
