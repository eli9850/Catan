#include <iostream>
#include "CatanClient/CatanClient.h"


int main()
{
    CatanClient client("127.0.0.1", "12345");
    client.start_game();

    return 0;
}


