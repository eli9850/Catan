#pragma once

enum class PlayerType;

enum class SettlementType
{
	NONE,
	REGULAR,
	CITY
};

class Settlement
{
public:
	Settlement(const SettlementType settlement_type, const PlayerType player_type);
	SettlementType get_settlement_type() const;
	PlayerType get_player_type() const;

private:
	SettlementType m_settlement_type;
	PlayerType m_player_type;
};
