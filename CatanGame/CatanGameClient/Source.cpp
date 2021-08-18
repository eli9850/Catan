//#include "CatanClient/CatanClient.h"

#include <iostream>

#include "GUI/GUIClient.h"
#include "Catan/Board/GameBoard/BasicBoard.h"
#include "CatanClient/CatanClient.h"

void start_gui(CatanUtils::QueueUtils::WaitQueue* q)
{
	while (true)
	{
		const auto a = q->pop_and_front();
		std::cout << a << std::endl;
	}
}

int main()
{
	CatanClient client("127.0.0.1", "12345");
	client.start_game();

	// CatanUtils::BasicBoard board;
	// board.create_board();
	//
	// auto q = std::make_shared<CatanUtils::QueueUtils::WaitQueue>();
	// GUIClient client(q);
	// client.create_catan_board(board.to_string());
	// std::thread t(start_gui, q.get());
	// client.start_game();

	return 0;
}
