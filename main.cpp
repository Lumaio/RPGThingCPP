#include <iostream>
#include "Game.h"

using namespace std;

int main()
{
	Game Game("spooky scary skeletons sending shivers down 'yer spine");
	Game.Init();
	Game.Start();
	return 0;
}
