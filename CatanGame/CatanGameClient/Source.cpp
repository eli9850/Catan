#include <iostream>
#include "CatanClient/CatanClient.h"
#include "GUI/GUIClient.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    GUIClient client(hInstance);
    //CatanClient client("127.0.0.1", "12345");
    //client.start_game();
    return 0;
}


