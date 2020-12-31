#include "settlement.h"

Settlement::Settlement(SettlementType settlement_type, PlayerType player_type):m_settlement_type(settlement_type), m_player_type(player_type){}

PlayerType Settlement::get_player_type() const {
	return m_player_type;
}

SettlementType Settlement::get_settlement_type() const {
	return m_settlement_type
}
