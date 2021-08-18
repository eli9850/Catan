#include <iostream>
#include "GameManager/BasicGameManager.h"
#include "Exceptions/Exceptions.h"

#include <set>

/*#include <sstream>
#include <iostream>
#include <chrono>
#include "Utils/StringUtils.h"
#include "Utils/MapUtils.h"

void test(BasicGameManager& manager) {
	std::cout << manager.get_board().to_string() << std::endl;

	std::cout << "\n\n\nnew settlement:\n";
	std::stringstream data;
	data << "something" << "\n" << "0,4";
	manager.handle_create_settlement(0, split(data.str(), "\n"));
	std::cout << manager.get_board().to_string() << std::endl;
	
	std::cout << "\n\n\nnew edge:\n";
	data.str(std::string());
	data << "something" << "\n" << "0,4";
	manager.handle_create_edge(0, split(data.str(), "\n"));
	std::cout << manager.get_board().to_string() << std::endl;

	std::cout << "\n\n\nfailed edge:\n";
	data.str(std::string());
	data << "something" << "\n" << "4,5";
	manager.handle_create_edge(2, split(data.str(), "\n"));
	std::cout << manager.get_board().to_string() << std::endl;
}

void test2(BasicGameManager& manager) {
	manager;

	std::cout << manager.get_board().to_string() << std::endl << "@@@@@@@@@@@@@@" << std::endl;

	std::stringstream data;

	for (size_t i = 0; i < 40; i++)
	{
		try {
			manager.handle_roll_dices(2, {});
		}
		catch (...) {

		}
	}
	
}
*/

int main()
{
	// std::set<CatanUtils::PlayerType> players;
	//
	// players.emplace(CatanUtils::PlayerType::BLUE);
	// players.emplace(CatanUtils::PlayerType::BLUE);
	// players.emplace(CatanUtils::PlayerType::GREEN);
	// players.emplace(CatanUtils::PlayerType::BLUE);
	// players.emplace(CatanUtils::PlayerType::RED);
	//
	// std::cout << players.size() << std::endl;
	//
	// for (const auto& a : players)
	// {
	// 	std::cout << static_cast<uint32_t>(a) << std::endl;
	// }
	//
	// std::cout << static_cast<uint32_t>(*(++players.begin()++)) << std::endl;
	// std::cout << static_cast<uint32_t>(*(players.begin()++)) << std::endl;
	// std::cout << static_cast<uint32_t>(*(players.begin()++)) << std::endl;
	
	try
	{
		BasicGameManager manager(3, "12345");
		manager.start_game();

		//test(manager);
		//test2(manager);
		//manager.handle_roll_dices(1, {});
	}
	catch (const CatanException& e)
	{
		std::cout << e.get_message() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error" << std::endl;
	}
	return 0;
}
