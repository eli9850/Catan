#include <iostream>
#include "Game/GameManager/BasicGameManager.h"
#include "Exceptions/Exceptions.h"

#include <sstream>
#include <iostream>
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

	data << "something" << "\n" << "2,6";
	manager.handle_create_settlement(0, split(data.str(), "\n"));
	data.str(std::string());

	data << "something" << "\n" << "2,8";
	manager.handle_create_settlement(1, split(data.str(), "\n"));
	data.str(std::string());

	data << "something" << "\n" << "2,4";
	manager.handle_create_settlement(0, split(data.str(), "\n"));
	data.str(std::string());

	manager.handle_roll_dices(2, {});

}


int main() {
	try {
		BasicGameManager manager(3, "12345");
		//test(manager);
		test2(manager);
		//manager.start_game();		
	}
	catch (const CatanException& e) {
		std::cout << e.get_message() << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknown error" << std::endl;
	}
	return 0;
}