#include "GameManager.h"

#include "Exceptions/GameManagerExceptions.h"

GameManager::GameManager(const uint32_t number_of_players) : m_turn_number(0), m_game_board(),
                                                             m_development_cards(
	                                                             NUMBER_OF_DEVELOPMENT_CARDS),
                                                             m_players(number_of_players)
{
	if (number_of_players > MAX_NUMBER_OF_PLAYERS)
	{
		throw TooManyPlayersError("There should be no more then 4 players");
	}
	for (uint32_t i = 0; i < m_players.size(); ++i)
	{
		m_players[i] = std::make_shared<Player>(static_cast<PlayerType>(i));
	}
	for (uint32_t i = 0; i < NUMBER_OF_ABUNDANCE_CARDS; ++i)
	{
		m_development_cards[i] = DevelopmentCards::ABUNDANCE_CARD;
	}
	for (uint32_t i = NUMBER_OF_ABUNDANCE_CARDS; i < NUMBER_OF_ABUNDANCE_CARDS +
	     NUMBER_OF_POINT_CARDS; ++i)
	{
		m_development_cards[i] = DevelopmentCards::POINT_CARD;
	}
	for (uint32_t i = NUMBER_OF_ABUNDANCE_CARDS + NUMBER_OF_POINT_CARDS; i <
	     NUMBER_OF_ABUNDANCE_CARDS + NUMBER_OF_POINT_CARDS + NUMBER_OF_KNIGHT_CARDS; ++i)
	{
		m_development_cards[i] = DevelopmentCards::KNIGHT_CARD;
	}
	for (uint32_t i = NUMBER_OF_ABUNDANCE_CARDS + NUMBER_OF_POINT_CARDS + NUMBER_OF_KNIGHT_CARDS; i
	     < NUMBER_OF_ABUNDANCE_CARDS + NUMBER_OF_POINT_CARDS + NUMBER_OF_KNIGHT_CARDS +
	     NUMBER_OF_ROAD_CARDS; ++i)
	{
		m_development_cards[i] = DevelopmentCards::ROAD_CARD;
	}
	for (uint32_t i = NUMBER_OF_ABUNDANCE_CARDS + NUMBER_OF_POINT_CARDS + NUMBER_OF_KNIGHT_CARDS +
		     NUMBER_OF_ROAD_CARDS; i < NUMBER_OF_ABUNDANCE_CARDS + NUMBER_OF_POINT_CARDS +
	     NUMBER_OF_KNIGHT_CARDS + NUMBER_OF_ROAD_CARDS + NUMBER_OF_MONOPOL_CARDS; ++i)
	{
		m_development_cards[i] = DevelopmentCards::MONOPOL_CARD;
	}
	
}

void GameManager::launch()
{
	get_players();
	before_start();
	play();
}


void GameManager::get_players()
{
	// TODO: need to do this
}

void GameManager::before_start()
{
	// TODO: need to do this
}

void GameManager::play()
{
	while (true)
	{
		uint32_t dice_number = roll_the_dice();

		m_turn_number++;
	}
}

bool GameManager::is_there_a_winner() const
{
	return true;
}

uint32_t GameManager::roll_the_dice() const
{
	return 7;
}
